<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
// carrega as funções EMAIL
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();



//ATIVIDADES QUE IRÃO VENCER
$sql = "select
			a.atiid,
			a.aticodigo,
			a.atidescricao,
			a.atidetalhamento as detalhamento,
			a.atidatainicio,
			a.atidatafim,
			a.atidatafim - NOW()::date as diferenca,
			a.atistatus,
			a.atiordem,
			a.atiidpai,
			a.esaid,
			a.atidataconclusao,
			CASE 
			WHEN (a.atitipoandamento = 'p' OR a.atitipoandamento IS NULL) THEN a.atiporcentoexec
			WHEN (a.atitipoandamento = 'q') THEN ( ( coalesce(a.atiquantidadeexec, 0) / a.atimetanumerica ) * 100 )
			END as atiporcentoexec,			
			a.atitipoandamento,
			a.atiquantidadeexec,
			a.atimetanumerica,
			a._atiprojeto,
			a._atinumero as numeroatividade,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			aga.graid,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos,
			a.atitipoandamento, 
			a.atimetanumerica,
			a.atiquantidadeexec,
			a.usucpfcadastro,
			u2.usunome as usunomecadastro
		from pde.atividade a
		inner join pde.estadoatividade ea on
			ea.esaid = a.esaid
		left join pde.usuarioresponsabilidade ur on
			ur.atiid = a.atiid and
			ur.rpustatus = 'A' and
			ur.pflcod = 593
		left join pde.atividadegrupoatividade aga on
			aga.atiid = a.atiid and aga.graid=1
		left join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod and
			pu.usucpf = ur.usucpf
		left join seguranca.usuario u on
			u.usucpf = pu.usucpf and
			u.suscod = 'A'
		left join seguranca.usuario u2 on
			u2.usucpf = a.usucpfcadastro
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod and
			ug.ungstatus = 'A'
		left join (
			select atiid, count(*) as restricoes
			from pde.observacaoatividade
			where obsstatus = 'A' and obssolucao = false
			group by atiid ) restricao on
				restricao.atiid = a.atiid
		left join (
			select atiid, count(*) as anexos
			from pde.anexoatividade
			where anestatus = 'A'
			group by atiid ) anexo on
				anexo.atiid = a.atiid
		where
			a._atiprojeto = 120363
			and a.atiid != a._atiprojeto
			and a.atistatus = 'A'
			and a.atidatafim IS NOT NULL
			and u.usucpf IS NOT NULL
			and a.atidatafim::date - NOW()::date <= 2
			and a.atidatafim::date - NOW()::date > 0
		order by _atiordem";


$atividades = $db->carregar($sql);

if($atividades[0]) {
	foreach($atividades as $atividade) {

		if( $atividade['usuemail'] ){
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - Demandas SEB";
			
			$mensagem->AddAddress( $atividade['usuemail'], $atividade['usunome'] );
			$mensagem->Body = "<p>Prezado(a) Diretor(a),</p>
							   <p>Falta(m) ".$atividade['diferenca']." dia(s) para expirar o prazo para execução da atividade de número ".$atividade['numeroatividade'].". 
							   O Gabinete da Secretaria de Educação Básica solicita que a atividade seja cumprida dentro do prazo assinalado.</p>
							   <p>Descrição da atividade cadastrada: ".$atividade['detalhamento'].".</p>
							   <p><font size=1><i>Esta é uma mensagem automática, por favor não responda.</i></font></p>";
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			
			echo $resp."<br>";
		
		}
		// A partir daqui verifico se tem equipe de apoio e mando o e-mail para eles.
		$sql = "select
					u.usucpf as codigo,
					u.usunome as usunome,
					u.usuemail
				from 
					seguranca.usuario u
				inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
				where
					ur.rpustatus = 'A' and
					ur.pflcod = '594' and
					atiid = ".$atividade['atiid']."
				order by u.usucpf";
		
		$arrApoio = $db->carregar( $sql );
		
		if( is_array( $arrApoio ) ){
			foreach( $arrApoio as $apoio ){
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= SIGLA_SISTEMA;
				$mensagem->From 		= "noreply@mec.gov.br";
				$mensagem->Subject 		= SIGLA_SISTEMA. " - Demandas SEB";
				
				$mensagem->AddAddress( $apoio['usuemail'], $apoio['usunome'] );
				$mensagem->Body = "<p>Prezado(a) Diretor(a),</p>
								   <p>Falta(m) ".$atividade['diferenca']." dia(s) para expirar o prazo para execução da atividade de número ".$atividade['numeroatividade'].". 
								   O Gabinete da Secretaria de Educação Básica solicita que a atividade seja cumprida dentro do prazo assinalado.</p>
								   <p>Descrição da atividade cadastrada: ".$atividade['detalhamento'].".</p>
								   <p><font size=1><i>Esta é uma mensagem automática, por favor não responda.</i></font></p>";
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				
				echo $resp."<br>";
			}
		}

	}
/*
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA. " - Demandas SEB";
	$mensagem->From 		= "noreply@mec.gov.br";
	$mensagem->AddAddress( $_SESSION['email_sistema'], "Victor Benzi" );
	$mensagem->Subject = SIGLA_SISTEMA. " - Demandas SEB";
	$mensagem->Body = "Todos os ".count($atividades)." e-mails foram enviados com sucesso";
	$mensagem->IsHTML( true );
	$mensagem->Send();
*/
}

//limpa variavel
unset($atividades);


//ATIVIDADES VENCIDAS
$sql = "select
			a.atiid,
			a.aticodigo,
			a.atidescricao,
			a.atidetalhamento as detalhamento,
			a.atidatainicio,
			a.atidatafim,
			to_char(a.atidatafim,'dd/mm/YYYY') as datafim,
			a.atidatafim - NOW()::date as diferenca,
			a.atistatus,
			a.atiordem,
			a.atiidpai,
			a.esaid,
			a.atidataconclusao,
			CASE 
			WHEN (a.atitipoandamento = 'p' OR a.atitipoandamento IS NULL) THEN a.atiporcentoexec
			WHEN (a.atitipoandamento = 'q') THEN ( ( coalesce(a.atiquantidadeexec, 0) / a.atimetanumerica ) * 100 )
			END as atiporcentoexec,			
			a.atitipoandamento,
			a.atiquantidadeexec,
			a.atimetanumerica,
			a._atiprojeto,
			a._atinumero as numeroatividade,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			aga.graid,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos,
			a.atitipoandamento, 
			a.atimetanumerica,
			a.atiquantidadeexec,
			a.usucpfcadastro,
			u2.usunome as usunomecadastro
		from pde.atividade a
		inner join pde.estadoatividade ea on
			ea.esaid = a.esaid
		left join pde.usuarioresponsabilidade ur on
			ur.atiid = a.atiid and
			ur.rpustatus = 'A' and
			ur.pflcod = 593
		left join pde.atividadegrupoatividade aga on
			aga.atiid = a.atiid and aga.graid=1
		left join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod and
			pu.usucpf = ur.usucpf
		left join seguranca.usuario u on
			u.usucpf = pu.usucpf and
			u.suscod = 'A'
		left join seguranca.usuario u2 on
			u2.usucpf = a.usucpfcadastro
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod and
			ug.ungstatus = 'A'
		left join (
			select atiid, count(*) as restricoes
			from pde.observacaoatividade
			where obsstatus = 'A' and obssolucao = false
			group by atiid ) restricao on
				restricao.atiid = a.atiid
		left join (
			select atiid, count(*) as anexos
			from pde.anexoatividade
			where anestatus = 'A'
			group by atiid ) anexo on
				anexo.atiid = a.atiid
		where
			a._atiprojeto = 120363
			and a.atiid != a._atiprojeto
			and a.atistatus = 'A'
			and a.atidatafim IS NOT NULL
			and u.usucpf IS NOT NULL
			and a.esaid in (1,2,3) --(1=Não Iniciado / 2=Em Andamento / 3=Suspenso)
			and a.atidatafim::date - NOW()::date < 0
		order by _atiordem";


$atividades = $db->carregar($sql);

if($atividades[0]) {
	foreach($atividades as $atividade) {

		if( $atividade['usuemail'] ){
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - Demandas SEB";
			
			$mensagem->AddAddress( $atividade['usuemail'], $atividade['usunome'] );
			$mensagem->Body = "<p>Prezado(a) Diretor(a),</p>
							   <p>O prazo para execução da atividade de número ".$atividade['numeroatividade']." está expirado desde ".$atividade['datafim'].". 
							   O Gabinete da Secretaria de Educação Básica solicita que a atividade seja cumprida o mais rapidamente possível.</p>
							   <p>Descrição da atividade cadastrada: ".$atividade['detalhamento'].".</p>
							   <p><font size=1><i>Esta é uma mensagem automática, por favor não responda.</i></font></p>";
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			
			echo $resp."<br>";
		
		}
		// A partir daqui verifico se tem equipe de apoio e mando o e-mail para eles.
		$sql = "select
					u.usucpf as codigo,
					u.usunome as usunome,
					u.usuemail
				from 
					seguranca.usuario u
				inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
				where
					ur.rpustatus = 'A' and
					ur.pflcod = '594' and
					atiid = ".$atividade['atiid']."
				order by u.usucpf";
		
		$arrApoio = $db->carregar( $sql );
		
		if( is_array( $arrApoio ) ){
			foreach( $arrApoio as $apoio ){
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= SIGLA_SISTEMA;
				$mensagem->From 		= "noreply@mec.gov.br";
				$mensagem->Subject 		= SIGLA_SISTEMA. " - Demandas SEB";
				
				$mensagem->AddAddress( $apoio['usuemail'], $apoio['usunome'] );
				$mensagem->Body = "<p>Prezado(a) Diretor(a),</p>
							   <p>O prazo para execução da atividade de número ".$atividade['numeroatividade']." está expirado desde ".$atividade['datafim'].". 
							   O Gabinete da Secretaria de Educação Básica solicita que a atividade seja cumprida o mais rapidamente possível.</p>
							   <p>Descrição da atividade cadastrada: ".$atividade['detalhamento'].".</p>
							   <p><font size=1><i>Esta é uma mensagem automática, por favor não responda.</i></font></p>";
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				
				echo $resp."<br>";
			}
		}

	}
/*
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA. " - Demandas SEB";
	$mensagem->From 		= "noreply@mec.gov.br";
	$mensagem->AddAddress( $_SESSION['email_sistema'], "Victor Benzi" );
	$mensagem->Subject = SIGLA_SISTEMA. " - Demandas SEB";
	$mensagem->Body = "Todos os ".count($atividades)." e-mails foram enviados com sucesso";
	$mensagem->IsHTML( true );
	$mensagem->Send();
*/
}



?>