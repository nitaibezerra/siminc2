<?php
$horaInicio = date( 'H:m:s' );
set_time_limit(30000);
ini_set("memory_limit", "3000M");

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

#conexao com sqlserver
$myServer = "mecsrv14";
$myUser = "sysdbgeral";
$myPass = "sysdbgeral";
$myDB = "DBSRH";

$s = @mssql_connect($myServer, $myUser, $myPass)
or die("Couldn't connect to SQL Server on $myServer");

$d = @mssql_select_db($myDB, $s)
or die("Couldn't open database $myDB");

$query = "SELECT cs.nu_matricula_siape, cs.nu_cpf, cs.no_servidor, ss.ds_situacao_servidor, 
				uo.SG_UNIDADE_ORGANIZACIONAL, o.ds_orgao, ce.ds_cargo_emprego,  fu.ds_funcao
			FROM TB_SRH_CADASTRO_SERVIDOR cs
			            inner join TB_SRH_SITUACAO_SERVIDOR ss 
			                        on cs.co_situacao_servidor = ss.co_situacao_servidor
			            left join TB_SRH_UNIDADE_ORGANIZACIONAL uo 
			                        on uo.co_uorg_lotacao_servidor = cs.co_uorg_lotacao_servidor
			            inner join TB_SRH_ORGAO o 
			                        on o.co_orgao = cs.co_orgao
			            left join TB_SRH_CARGO_EMPREGO ce 
			                        on ce.co_cargo_emprego = cs.co_cargo_emprego
			            left join TB_SRH_FUNCAO fu 
			                        on fu.co_funcao = cs.co_funcao";

$result = mssql_query($query);
$numRows = mssql_num_rows($result);
$countRegistro = 0;
$sql = "DELETE FROM siscap.tb_cadastro_servidor";
$db->executar( $sql );

$sql = "SELECT nu_cpf FROM siscap.tb_cadastro_servidor";
$arNu_cpf = $db->carregarColuna( $sql );
$arNu_cpf = $arNu_cpf ? $arNu_cpf : array();

while($row = mssql_fetch_array($result)){
	$nu_cpf = "'".str_replace(array(".", "-"), "", trim($row["nu_cpf"]))."'";

		$nu_matricula_siape 		= "'".trim($row["nu_matricula_siape"])."'";
		$co_uorg_lotacao_servidor	= "'".trim($row["co_uorg_lotacao_servidor"])."'"; 
		$no_servidor 				= "'".str_replace( "'", "\'", trim($row["no_servidor"]) )."'";
		$co_funcao 					= "'".trim($row["co_funcao"])."'";
		$co_nivel_funcao 			= "'".trim($row["co_nivel_funcao"])."'";
		$ds_funcao 					= "'".trim($row["ds_funcao"])."'";
		$ds_situacao_servidor 		= "'".trim($row["ds_situacao_servidor"])."'"; 
		$co_orgao_lotacao_servidor 	= "'".trim($row["co_orgao_lotacao_servidor"])."'";
		$sg_unidade_organizacional 	= "'".trim($row["sg_unidade_organizacional"])."'";
		$ds_orgao 					= "'".trim($row["ds_orgao"])."'";
		$ds_cargo_emprego 			= "'".trim($row["ds_cargo_emprego"])."'";
	  	
		$sql = "INSERT INTO siscap.tb_cadastro_servidor(nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor, co_funcao, co_nivel_funcao,
	  				ds_funcao, ds_situacao_servidor, co_orgao_lotacao_servidor, sg_unidade_organizacional, ds_orgao, ds_cargo_emprego, nu_cpf) 
				VALUES ($nu_matricula_siape, $co_uorg_lotacao_servidor, $no_servidor, $co_funcao, $co_nivel_funcao,
	  				$ds_funcao, $ds_situacao_servidor, $co_orgao_lotacao_servidor, $sg_unidade_organizacional, $ds_orgao, $ds_cargo_emprego, $nu_cpf)";
	  				
	  	$db->executar( $sql );
	  	$countRegistro++;
}
if($db->commit()){
	echo '<pre>';
	echo "Total de registro inseridos: $countRegistro";
	echo '</pre>';
	echo '<br>';
}
$horaFim = date('H:m:s');

$formatoini = strtotime($horaInicio);
$formatofim = strtotime($horaFim);
$converte = $formatofim - $formatoini;
echo '<pre>';
echo 'Hora Inicio: '.$horaInicio.'<br>';
echo 'Hora Inicio: '.$horaFim.'<br>';
echo 'Segundos: '.$converte.'<br>';
echo "Tempo de execução: ".($converte/60);
echo '</pre>';
echo '<br>';
?>