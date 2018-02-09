<?php

/**
 * Avisa usuÃ¡rios responsÃ¡veis por processo que estÃ¡ a 1 dia do prazo de resposta externa.
 *
 * DiÃ¡rio - 09:00 AM
 * Arquivo: cron_conjur_informa_prazos_processo.php
 */


date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funÃ§Ãµes gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexÃ£o com o servidor de banco de dados
$db = new cls_banco();


// busca processos que atendem ao prazo
$sql = " SELECT 
			prc.prcdesc, 
			prc.prcnumsidoc, 
			u.usunome, 
			u.usuemail,
			u2.usunome AS usunomeseres,
			u2.usuemail AS usuemailseres,
			MAX(pri.dt_delegacao) as maior_data
		FROM conjur.estruturaprocesso e
		INNER JOIN workflow.documento doc ON doc.docid = e.docid
		INNER JOIN conjur.processoconjur prc ON prc.prcid = e.prcid
		INNER JOIN conjur.respostaexterna pri ON pri.prcid = prc.prcid
		
		INNER JOIN seguranca.usuario u ON u.usucpf = pri.usucpf

		LEFT JOIN gestaodocumentos.tarefa t ON t.tarnumsidoc = prc.prcnumsidoc
		LEFT JOIN seguranca.usuario u2 ON u2.usucpf = t.usucpfresponsavel

		WHERE 
			date_part('day',e.espdtrespexterna) - date_part('day',NOW()) = 1
		GROUP BY prc.prcdesc, 
				 prc.prcnumsidoc, 
				 u.usunome, 
				 u.usuemail, 
				 pri.dt_delegacao,
				 u2.usunome,
			 	 u2.usuemail ";

$processosNoPrazo = $db->carregar( $sql );

//ver($processosNoPrazo, d);
if($processosNoPrazo[0]) {
	
	foreach ($processosNoPrazo as $key => $value) {
		
		if( empty($value['prcdesc']) ) continue;
		if( empty($value['prcnumsidoc']) ) continue;
		if( empty($value['usunome']) ) continue;
		if( empty($value['usuemail']) ) continue;
	
		$assunto = "Informe Conjur - Prazo de Processo Aguardando resposta externa vencendo";
		$mensagem = " 
			Prezados,<br/>
			<br/>
			Esse email é um informativo de que o processo especificado a seguir esté a 1 dia do prazo de retorno da resposta externa.<br/>
			Dados:<br/>
			Descrição: '".$value['prcdesc']."'.<br/>
			Número SIDOC: '".$value['prcnumsidoc']."'.<br/>
			<br/>
			Atenciosamente,<br/>
			Equipe ". SIGLA_SISTEMA. ". ";
	
		$remetente = '';
		
		//ver($assunto, $mensagem, $remetente, d);
		// ENVIO DE EMAIL PARA PERFIL 'EXTERNO CONJUR'
		
		$usuTeste = 'Victor Martins Machado';
		$emailTeste = $_SESSION['email_sistema'];
		
		$destinatario = array('usunome'=>$value['usunome'],'usuemail'=>$value['usuemail']);
		// Destinatário utilizado para testes no ambiente de desenvolvimento
		//$destinatario = array('usunome'=>$usuTeste,'usuemail'=>$emailTeste);
		enviar_email( $remetente, $destinatario, $assunto, $mensagem );
	
		// ENVIO DE EMAIL PARA RESPONSAVEL POR DEMANDA SERES
		if( empty($value['usunomeseres']) ) continue;
		if( empty($value['usuemailseres']) ) continue;
		$destinatario = array('usunome'=>$value['usunomeseres'],'usuemail'=>$value['usuemailseres']);
		// Destinatário utilizado para testes no ambiente de desenvolvimento
		//$destinatario = array('usunome'=>$usuTeste,'usuemail'=>$emailTeste);
		enviar_email( $remetente, $destinatario, $assunto, $mensagem );
	}
}

?>