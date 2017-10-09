<?php
ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once "../_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct emenda, r.uo, r.nome_uo, r.cnpj, r.nome, r.cpf, r.cargo, r.telefone, r.email, ede.emdid
		from carga.responsavel_emenda r
		    inner join emenda.emenda e on e.emecod = r.emenda
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
		    inner join emenda.entidadebeneficiada eb on eb.enbcnpj = r.cnpj
		where e.emeano = '2015' --and e.etoid = 4
			and ede.edestatus = 'A' 
			--and e.emecod = '19560017'";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
//ver($arrDados, d);
$totalAtualizada = 0;
$emendaNaoAtualizada = array();
$emendaAtualizada = array();

foreach ($arrDados as $v) {
	$v['telefone'] = trim(str_replace(array('(', ')', '-', ' '), '', $v['telefone']));
	$v['cpf'] = trim(str_replace(array('.', '-', ' '), '', $v['cpf']));
	$v['cnpj'] = trim(str_replace(array('.', '-', ' '), '', $v['cnpj']));
	
	if( strlen($v['telefone']) > 8 ){
		$v['dd'] = substr($v['telefone'], 0, 2);
		$v['telefone'] = substr($v['telefone'], 2);
	}
		
	$enbid = $db->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$v['cnpj']."' and enbano = '".date('Y')."'");
	
	
	if( !empty($enbid) ){
		
		$boContra = $db->pegaUm("select count(enbid) from emenda.entbeneficiadacontrapartida where enbid = $enbid");
		if( $boContra < 1 ){
			$sql = "INSERT INTO emenda.entbeneficiadacontrapartida(enbid, ebcexercicio, ebccontrapartida) 
					VALUES ($enbid, '".date('Y')."', 0)";
			$db->executar($sql);
		}
		
		$boUserResp = $db->pegaUm("select count(rpuid) from emenda.usuarioresponsabilidade where pflcod = 274 and usucpf = '{$v["cpf"]}' and enbid = $enbid and rpustatus = 'A'");
		if( $boUserResp < 1 ){
			$sql = "INSERT INTO emenda.usuarioresponsabilidade(pflcod, usucpf, enbid, rpustatus, rpudata_inc) 
					VALUES (274, '{$v["cpf"]}', $enbid, 'A', now())";
			$db->executar($sql);
		}
		$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = {$v["emdid"]} and enbid = $enbid and edestatus = 'A'");
		if( empty($edeid) ){
			array_push($emendaNaoAtualizada, array('emecod' => $v['emenda'], 'unicod' => $v['uo'], 'unidsc'=>$v['nome_uo']) );
			/* $sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, usucpfalteracao, ededataalteracao, edecpfresp, edenomerep, edemailresp, ededddresp, edetelresp, edestatus, ededisponivelpta )
					VALUES ( {$v["emdid"]}, {$enbid}, '{$v["emdvalor"]}', '', 'now()', '{$arrResp["usucpf"]}', '{$arrResp["nomeusuario"]}', '{$arrResp["usuemail"]}', '{$arrResp["dd"]}', '{$arrResp["fone"]}', 'A', 'S' ) RETURNING edeid";
			$edeid = $db->pegaUm( $sql ); */
		} else {
			
			$sql = "UPDATE emenda.emendadetalheentidade SET 
						edecpfresp = '{$v["cpf"]}',
						edenomerep = '{$v["nome"]}',
						edemailresp = '{$v["email"]}',
						ededddresp = '{$v["dd"]}', 
						edetelresp = '{$v["telefone"]}',
						ededisponivelpta = 'S'
					WHERE edeid = $edeid";
			$db->executar($sql);
			
			array_push($emendaAtualizada, array('emecod' => $v['emenda'], 'unicod' => $v['uo'], 'unidsc'=>$v['nome_uo']) );
		}
		
		$boUserSis = $db->pegaUm("select count(u.usucpf) from seguranca.usuario_sistema u where u.usucpf = '{$v["cpf"]}' and sisid = 57");
		$boPerUser = $db->pegaUm("select count(p.usucpf) from seguranca.perfilusuario p where p.usucpf = '{$v["cpf"]}' and pflcod = 274");
		
		if( $boPerUser < 1 ){
			$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) 
					VALUES ('{$v["cpf"]}', 274)";
			$db->executar($sql);
		}
		
		if( $boUserSis < 1 ){
			$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) 
					VALUES ('{$v["cpf"]}', 57, 'A', 274, now(), 'A')";
			$db->executar($sql);
		} else {
			$sql = "UPDATE seguranca.usuario_sistema SET susstatus = 'A', suscod = 'A' WHERE usucpf = '{$v["cpf"]}' and sisid = 57";
			$db->executar($sql);
		}
		$db->commit();
		
		$totalAtualizada++;
	} else {
		array_push($emendaNaoAtualizada, array('emecod' => $v['emenda'], 'unicod' => $v['uo'], 'unidsc'=>$v['nome_uo']) );
	}
}
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Recursos</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"><b>Total de Emendas V1:</b></td>
		<td><?=sizeof($arrDados); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"><b>Total de Emendas Atualizadas:</b></td>
		<td><?=$totalAtualizada; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Emendas Atualizadas</b></td>
	</tr>
	<tr>
		<td colspan="2">
		<?
		$cabecalho = array('Emenda', 'Unicod', 'Unidade');
		$db->monta_lista_simples($emendaAtualizada, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Emendas não Atualizadas</b></td>
	</tr>
	<tr>
		<td colspan="2">
		<?		
		$cabecalho = array('Emenda', 'Unicod', 'Unidade');
		$db->monta_lista_simples($emendaNaoAtualizada, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
		?>
		</td>
	</tr>
</table>
<?

if( $_REQUEST['enviaEmail'] == 'S' ){
	$sql = "select distinct emenda, a.autcod
			from carga.responsavel_emenda r
			    inner join emenda.emenda e on e.emecod = r.emenda
			    inner join emenda.autor a on a.autid = e.autid
			    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
			    inner join emenda.entidadebeneficiada eb on eb.enbcnpj = r.cnpj
			where e.emeano = '2015' --and e.etoid = 4
				and ede.edestatus = 'A'";
	$arrDadosEmail = $db->carregar($sql);
	$arrDadosEmail = $arrDadosEmail ? $arrDadosEmail : array();
	//ver($arrDadosEmail, d);
	
	foreach ($arrDadosEmail as $v) {
	
		$conteudo = '<p><b>Senhor(a) parlamentar,</b></p>
A indicação da emenda '.$v['emenda'].'/'.date(Y).' foi validada no SIOP.<br>
O próximo passo é o preenchimento, até 07/08/'.date(Y).' no SIMEC/Emendas da iniciativa, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação.<br>
Qualquer dúvida, tratar com a ASPAR do MEC (2022-7899/7896/7894)';

		$remetente = array('nome' => SIGLA_SISTEMA. ' - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
			
		$email = $db->pegaUm("select a.autemail from emenda.autor a where a.autcod = '{$v['autcod']}'");
		
		if( !empty($email) ){
			$retorno = enviar_email($remetente, array($email), SIGLA_SISTEMA. ' - EMENDAS', $conteudo, $cc, null);
		}
	}
}

?>