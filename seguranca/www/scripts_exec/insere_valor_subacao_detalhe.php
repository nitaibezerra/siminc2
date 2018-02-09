<?php

/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'www/par/_funcoesPar.php';

$dataInicio = date("d/m/Y h:i:s"); 

$db 				= new cls_banco();

$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';


if($_GET['deletar'])
{
	die('asdf22222222');
	$db->executar("
	delete from carga.cargavaloressubacaodetalhe "); 
	$db->commit();
}
else if($_GET['update'])
{
	
	$db->executar("
			UPDATE 
				par.subacaodetalhe 
			SET
				sbdvalorplanejado = foo.sbdvalorplanejado,
				sbdvaloraprovado = foo.sbdvaloraprovado
		
			from (
				
				SELECT 
					sbdid,
					sbdvalorplanejado,
					sbdvaloraprovado
				
				FROM
					carga.cargavaloressubacaodetalhe 
				WHERE 
					sbdid not in(
						select distinct sbdid from par.subacaodetalhe where sbdvalorplanejado is not null OR sbdvaloraprovado is not null
					)
				
		
			)  foo
			WHERE foo.sbdid = par.subacaodetalhe.sbdid;

			
		");
	$db->commit();
}
else
{
	/*
	$db->executar("
	INSERT into carga.cargavaloressubacaodetalhe
		
		SELECT 
			sd.sbdid  as sbdid
			, par.recuperavalorplanejadossubacaoporano(sd.sbaid, sd.sbdano) as sbdvalorplanejado,
			par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano) as sbdvaloraprovado
		
		FROM
			par.subacaodetalhe sd;
			
"); 
$db->commit();*/
}



$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

$html = "<span style='color: red;'><b>Detalhes da Execução:</b><br/><br/>
									<b>Rotina de Carga de Subação ".$dataInicio." a ".$dataFim.",<br>realizada com sucesso em ".$intervalos.".</b></span>";
$assunto  = SIGLA_SISTEMA. " - Carga Subação Detalhe";

enviar_email(array('nome'=>SIGLA_SISTEMA. ' - CARGA SUBAÇÃO DETALHE - '.$ano, 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );

$db->close();

die('fim');
exit();
?>