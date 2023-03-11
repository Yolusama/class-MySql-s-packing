#include "MySql.h"
using namespace MySql_Con;

MySql::MySql()
{
	mysql_init(&mysql);
}

MySql::MySql(String host, String user, String pwd, String database, unsigned port, String socket, size_t flag)
{
	mysql_init(&mysql);
	this->host = host;
	this->user = user;
	this->pwd = pwd;
	this->database = database;
	this->port = port;
	Connect(socket,flag);
}

void MySql::SetCharset(String charset)
{
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, charset.data());
}

void MySql::SetOption(mysql_option opt, String option)
{
	mysql_options(&mysql, opt, option.data());
}

int MySql::MySqlQuery(String query)
{
	return mysql_query(&mysql, query.data());
}

MySqlCommand MySql::CreateCommnad()
{
	return MySqlCommand("",*this);
}

std::list<String> MySqlDataReader::fields_name()
{
		std::list<String> vec;
		MYSQL_FIELD* fields = mysql_fetch_fields(res);
		if (row == NULL)return std::list<String>();
		for(unsigned i=0;i<count();i++)
			vec.push_back(String(fields[i].name));
		return vec;
}

bool MySqlDataReader::read()
{
	row = mysql_fetch_row(res);
	return row!=NULL;
}

int MySqlDataReader::count()
{
	return mysql_num_fields(res);
}

void MySqlDataReader::close()
{
	mysql_free_result(res);
}

String MySqlDataReader::getString(unsigned index)
{
	if (row == NULL||index>=count())std::abort();
	return String(row[index]);
}

MySqlCommand::MySqlCommand(String command, MySql conection)
{
	commandText = command;
	con = conection;
}
int MySqlCommand::execQuery()
{
	int i = con.MySqlQuery(commandText);
	reader = con.LastExeResult();
	return i;
}