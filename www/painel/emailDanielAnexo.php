<?php
include_once "config.inc";
include_once "_constantes.php";

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("Diligência - Proinfância e Quadras");
$html = 'Senhor(a) Dirigente Municipal,<br /><br />
 

O seu município cadastrou e enviou projeto(s) de infraestrutura referente(s) ao PAC 2 (Proinfância e/ou construção de quadras cobertas) pelo Simec - Módulo PAR 2010.<br /><br />
 

Após a análise do FNDE, verificamos que há proposta(s) na situação "em diligência".<br /><br />


Solicitamos que a equipe municipal acesse o PAR 2010, clique na obra que está em diligência e, depois, na aba "Análise de Engenharia" (abrir todos).<br /><br /> 


Nos itens da análise de engenharia em que a resposta é "não", deve-se ler a "Observação", ajustar o que é solicitado e tramitar para nova análise até às 23horas e 59 minutos do dia 09 de dezembro de 2010. Após esta data o sistema será fechado para recebimento de resposta das diligências, resultando no indeferimento da ação nesta etapa do PAC 2.<br /><br />
 

Caso o município tenha outro(s) projeto(s) que se encontra(m) na situação "Aguardando análise - FNDE", a equipe municipal deve acompanhar a situação. Se essa(s) obra(s) entrar(em) "em diligência", o mesmo procedimento deve ser seguido.<br /><br />

 
Atenciosamente,<br /><br />


Equipe Técnica do PAR';
echo $html;
$e->setText($html);
$e->setName("Diligência - Proinfância e Quadras");
$e->setEmailOrigem($_SESSION['email_sistema']);
$e->addAnexo(APPRAIZ."www/painel/emailsDaniel.txt");
$e->addAnexo(APPRAIZ."www/painel/email.txt");
$e->setEmailsDestinoPorArquivo(APPRAIZ."www/painel/emailsDanielAnexo.txt");
$e->setEmailsDestino(array("julianomeinen.souza@gmail.com"));
$e->enviarEmails();