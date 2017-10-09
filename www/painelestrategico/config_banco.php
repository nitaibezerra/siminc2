<?php

/*
 * Desenvolvido por: FGV Projetos
 * Data: 28/02/09
 * Programa: config_banco.php
 * Descrição sumária: abre uma conexão com o banco de dados.
 */

$conexao = pg_connect("host= dbname=  port= user= password=");
pg_query("SET search_path = mec_painel, pg_catalog;");
pg_set_client_encoding('LATIN5');

if (!$conexao)
{

//no caso de erro, enviar e-mail avisando

//configurações do e-mail

include "class.phpmailer.php";

$mail = new PHPMailer();
$mail->IsSMTP(); //ENVIAR VIA SMTP
$mail->Host = "smtp.xxx.gov.br"; //SERVIDOR DE SMTP, USE smtp.SeuDominio.com OU smtp.hostsys.com.br
$mail->SMTPAuth = true; // ATIVA O SMTP AUTENTICADO
$mail->Username = "yyy@xxx.gov.br"; //EMAIL PARA SMTP AUTENTICADO (pode ser qualquer conta de email do seu domínio)
$mail->Password = "zzzzzzz"; //SENHA DO EMAIL PARA SMTP AUTENTICADO
$mail->From = "zzz@xxx.gov.br"; //E-MAIL DO REMETENTE
$mail->FromName = SIGLA_SISTEMA; //NOME DO REMETENTE
$mail->IsHTML(true); //ATIVA MENSAGEM NO FORMATO HTML
$mail->Subject = "ERRO CONEXÃO COM BANCO DE DADOS"; //ASSUNTO DA MENSAGEM

$to = "aaa@xxx.gov.br";

$mail->AddAddress($to); //E-MAIL DO DESINATÁRIO
$mail->Body = "Erro ao conectar com HOST $host e usuário $usuario"; //MENSAGEM NO FORMATO HTML
$mail->Send();


echo "Eror ao conectar com o banco de dados";

@pg_close( $conexao );
exit();

}


?>
