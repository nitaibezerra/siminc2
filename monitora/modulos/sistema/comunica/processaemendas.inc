<?php 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 1
   */

class FuncionalProgramaticaInvalidaException extends Exception {}
class CodigoAcaoNaoEncontradoException extends Exception {}

if(!$_FILES["arqEmendas"]) {
	return false;
	exit;
}

set_time_limit(0);

$fp = fopen($_FILES["arqEmendas"]["tmp_name"], "r");
$campos = fgetcsv($fp, 1024, ";", "\"");

$pgconn = pg_connect("host=".$servidor_bd." port=".$porta_bd." dbname=".$nome_bd."  user=".$usuario_db." password=".$senha_bd);
pg_set_client_encoding($pgconn,'LATIN1');
pg_query($pgconn, 'begin;');

//
// constantes
define("TESTE", isset($testaupload) ? $testaupload : false);
define("PFUNCPROG", 12);
define("PUNICOD", 10);
define("PEMDCOD", 9);
define("PCODAUTOR", 18);
define("PCODTIPOAUTOR", 2);
define("PUFAUTOR", 4);
define("PNOMEAUTOR", 8);
define("PSGLPARTIDOAUTOR", 5);
define("PSALDO", 16);
define("PGND", 22);
define("PFONTE", 21);
define("PMODAPLICACAO", 23);

//
// variaveis
$data = date("Y-m-d");
$exercicio = $_SESSION["exercicio"];
$erroacaid=0;

$sqlSelAcao = "SELECT acaid FROM monitora.acao WHERE unicod = '%s' AND prgcod = '%s' AND acacod = '%s' AND loccod = '%s' AND prgano = '$exercicio'";
$sqlUpdAcao = "UPDATE monitora.acao SET acasnemenda = 't' WHERE acaid = '%s' AND prgano = '$exercicio'";
$sqlUpdAcaoFuncao = "UPDATE monitora.acao SET funcod = '%s' WHERE acaid = '%s' AND funcod IS NULL OR funcod = '' AND prgano = '$exercicio'";
$sqlUpdAcaoSubFuncao = "UPDATE monitora.acao SET sfucod = '%s' WHERE acaid = '%s' AND sfucod IS NULL OR sfucod = '' AND prgano = '$exercicio'";
$sqlSelEmenda = "SELECT emdid FROM monitora.emenda WHERE emdcod = '%s' AND emdcodautor = '%s' AND foncod = '%s' AND gndcod = '%s' AND mapcod = '%s' AND prgano = '$exercicio'";
$sqlSelEmendaOID = "SELECT emdid FROM monitora.emenda WHERE oid = '%s'";
$sqlInsEmenda = "INSERT INTO monitora.emenda (
						acaid,
						emdcod,
						emdcodautor,
						emdcodtipoautor,
						emdufautor,
						emdnomeautor,
						emdsglpartidoautor,
						emdsaldo,
						gndcod,
						foncod,
						mapcod,
						prgano
					) VALUES (
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s',
						'$exercicio'
				)";
$acasnemenda = array();
while($row = fgetcsv($fp, 1000, ";", "\"")) {
try {
//	var_dump($row);exit;
	$funcprog = explode(".", $row[PFUNCPROG]);
	if(@count($funcprog)<5) {
		throw new FuncionalProgramaticaInvalidaException("QUANTIDADE DE CAMPOS ERRADA NA FUNCIONAL PROGRAMATICA (" . count($funcprog) . "): " . $row[10]);
	}

	$query = sprintf($sqlSelAcao, $row[PUNICOD], $funcprog[2], $funcprog[3], $funcprog[4]);
	if(TESTE)
		echo "$query\n";
	$rs = @pg_query($query) or die("ERRO CONSULTA: $query");
	$acaid = @pg_result($rs, 0, "acaid");
	if(!(boolean)$acaid) {
		throw new CodigoAcaoNaoEncontradoException("CÓDIGO DA AÇÃO NÃO ENCONTRADO: $query");
	}

	$query = sprintf($sqlUpdAcao, $acaid);
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0) {
		echo "ACAO $acaid ATUALIZADA acasnemenda='t' ACAID=(" . $acaid . ")\n";
		$acasnemenda[$acaid] = 't';
	}

	$query = vsprintf($sqlUpdAcaoFuncao, array($funcprog[0]	// código função
												,$acaid));
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0)
		echo "ACAO $acaid ATUALIZADA FUNCOD=(" . $funcprog[0] . ")\n";

	$query = vsprintf($sqlUpdAcaoSubFuncao, array($funcprog[1]	// código subfunção
												,$acaid));
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_affected_rows($rs)>0)
		echo "ACAO $acaid ATUALIZADA SFUCOD=(" . $funcprog[1] . ")\n";
		
	$query = vsprintf($sqlSelEmenda, array($row[PEMDCOD]
											,$row[PCODAUTOR]
											,$row[PFONTE]
											,$row[PGND]
											,$row[PMODAPLICACAO]));
	if(TESTE)
		echo "$query\n";
	$rs = pg_query($query);
	if(pg_num_rows($rs)>0) {
		$emdid = pg_result($rs, 0, "emdid");
		echo "EMENDA ($emdid) JÁ EXISTE\n";
	}
	else {
		$dadosEmenda = array($acaid
			,$row[PEMDCOD]
			,$row[PCODAUTOR]
			,$row[PCODTIPOAUTOR]
			,$row[PUFAUTOR]
			,mb_convert_encoding($row[PNOMEAUTOR],'ISO-8859-1')
			,$row[PSGLPARTIDOAUTOR]
			,$row[PSALDO]
			,$row[PGND]
			,$row[PFONTE]
			,$row[PMODAPLICACAO]
		);

		$query = vsprintf($sqlInsEmenda, $dadosEmenda);
		if(TESTE)
			echo "$query\n";
		$rs = pg_query($query);
		if(!$rs) {
			throw new Exception("Erro ao inserir emenda: $query\n" . pg_errormessage($pgconn));
		}
		
		$oid = pg_last_oid($rs);
		$query = sprintf($sqlSelEmendaOID, $oid);
		if(TESTE)
			echo "$query\n";

		$rs = pg_query($query);
		
		$emdid = pg_result($rs, 0, "emdid");
		if(!(bool)$emdid) {
			throw new Exception("Não foi possível recuperar a emenda inserida OID: $oid\nCONSULTA: $query\n" . pg_errormessage($pgconn));
		}
	}
	
	// execucao
/*	$query = sprintf($sqlSelExecucao, $emdid, $data);
	if(TESTE)
		echo "$query\n";

	$rs = pg_query($query);
	if(pg_num_rows($rs)>0) {
		$query = sprintf($sqlDelExecucao, $emdid, $data);
		if(TESTE)
			echo "$query\n";
		if(!pg_query($query)) {
			throw new Exception("ERRO AO EXCLUIR EXECUÇÃO PARA A EMENDA: $emdid ($data)\n$query");
		}
	}
	
	$dadosExecução = array($data
		,$emdid
		,number_format((float)$row[14], '2', '.', '')
		,number_format((float)$row[15], '2', '.', '')
		,number_format((float)$row[16], '2', '.', '')
		,number_format((float)$row[17], '2', '.', '')
		,number_format((float)$row[18], '2', '.', '')
	);
	$query = vsprintf($sqlInsExecucao, $dadosExecução);
	if(TESTE)
		echo "$query\n";

	if(!pg_query($query)) {
		throw new Exception("ERRO AO INSERIR EXECUÇÃO PARA A EMENDA: $emdid ($data)\n$query");		
	}
*/
	printf("LINHA: %100d OK\n", $linha++);
	if(TESTE)
		echo "\n";
	flush();
	}
	catch(CodigoAcaoNaoEncontradoException $e) {
		printf("\nERRO! %s\n\n", $e->getMessage());
		$erroacaid++;
		continue;
	}
	catch(Exception $e) {
		var_dump($dados);
		var_dump($row);
		var_dump($e);
		die();
	}
}

pg_query($pgconn, "UPDATE monitora.acao SET acasnemenda = 't' WHERE acaid IN (SELECT DISTINCT acaid FROM emenda.emenda)");
var_dump("UPDATE monitora.acao SET acasnemenda = 't' WHERE acaid IN (SELECT DISTINCT acaid FROM dbemd.emenda WHERE prgcod = '$exercicio')");

if(TESTE)
	pg_query($pgconn, 'rollback;');
else
	pg_query($pgconn, 'commit;');

pg_close($pgconn);
?>
</pre>