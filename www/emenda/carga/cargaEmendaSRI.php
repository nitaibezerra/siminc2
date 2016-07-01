<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "../_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
//require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
//require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT autor, tipo_autor, partido, uf, emenda, categoria_emenda, uo, nome_uo, funcional, titulo, funcao, subfuncao,
  			programa, acao, loc, rp, esfera, fonte, mod, gnd, valor_emenda, liberacao_sri
		FROM carga.emenda_liberacao_sri where emenda not in ('27360004')";

$arrEmendaSRI = $db->carregar( $sql );
$arrEmendaSRI = $arrEmendaSRI ? $arrEmendaSRI : array();

$sql = "SELECT autor, partido, uf, emenda, orgao, funcional, objeto, rp, gnd, mod, fonte, cnpj, beneficiario, valor
		FROM carga.emenda_liberacao_sri_entidade_fnde where emenda not in ('27360004')";

$arrEmendaSRIEntidade = $db->carregar( $sql );
$arrEmendaSRIEntidade = $arrEmendaSRIEntidade ? $arrEmendaSRIEntidade : array();

$emedaDetalhe = 0;
#Carga de liberação na emenda detalhe
foreach ($arrEmendaSRI as $v) {
	$valor = str_replace(".","", $v['liberacao_sri']);
	$valor = str_replace(",",".", $valor);
		
	$sql = "UPDATE emenda.emendadetalhe SET emdliberacaosri = {$valor} 
			WHERE
				emeid = (select emeid from emenda.emenda where emecod = '{$v['emenda']}' and emeano = '2014')
			  	and emdimpositiva = 6
			  	and gndcod = '{$v['gnd']}'
  				and foncod = '{$v['fonte']}'
  				and mapcod = '{$v['mod']}'";
	$db->executar($sql);
	$db->commit();
	
	$emedaDetalhe++;
}

$edevalorreducao1 = 0;
foreach ($arrEmendaSRIEntidade as $v) {
	$valor = str_replace(".","", $v['valor']);
	$valor = str_replace(",",".", $valor);
	
	$sql = "select emdid from emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid  
			where emecod = '{$v['emenda']}' 
				and e.emeano = '2014'
				and ed.emdimpositiva = 6
				and ed.gndcod = '{$v['gnd']}'
  				and ed.foncod = '{$v['fonte']}'
  				and ed.mapcod = '{$v['mod']}'";
	$emdid = $db->pegaUm($sql);
	
	$cnpj = str_replace(array(".","/","-"), "", $v['cnpj']);
	$cnpj = str_ireplace(' ', '', $cnpj);
	
	if( $emdid ){		
		$enbid = $db->pegaUm("select enbid from emenda.entidadebeneficiada where enbcnpj = '{$cnpj}' and enbano = '2014' and enbstatus = 'A'");
		
		if( !empty($enbid) ){
		
			$sql = "UPDATE emenda.emendadetalheentidade SET edevalorreducao = (edevalor - {$valor}), edecarga = 1
					WHERE
						emdid = $emdid
						and edestatus = 'A'
						and enbid = $enbid
						and edecarga is null";
			$db->executar($sql);
			$db->commit();
			
			$edevalorreducao1++;
		}
	}
}
$edevalorreducao2 = 0;
$edevalorreducao3 = 0;
#Atualiza todas entidades que fazem parte das emendas com liberação zerada na planilha 1
foreach ($arrEmendaSRI as $v) {
	$valor = str_replace(".","", $v['liberacao_sri']);
	$valor = str_replace(",",".", $valor);
	
	$sql = "select emdid from emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid  
			where emecod = '{$v['emenda']}' 
				and e.emeano = '2014'
				and ed.emdimpositiva = 6
				and ed.gndcod = '{$v['gnd']}'
  				and ed.foncod = '{$v['fonte']}'
  				and ed.mapcod = '{$v['mod']}'";
	$emdid = $db->pegaUm($sql);
		
	if( $emdid ){
		$sql = "select edeid, edevalor from emenda.emendadetalheentidade where emdid = $emdid and edestatus = 'A'";
		$arEntidade = $db->carregar($sql);
		$arEntidade = $arEntidade ? $arEntidade : array();
			
		if( $valor == 0 ){			
			foreach ($arEntidade as $entidade) {
				$sql = "UPDATE emenda.emendadetalheentidade SET edevalorreducao = {$entidade['edevalor']}, edecarga = 2
						WHERE
							emdid = $emdid
							and edeid = {$entidade['edeid']}
							and edestatus = 'A'
							and edecarga is null";
				$db->executar($sql);
				$edevalorreducao2++;
			}
			$db->commit();
		}
		
		$sql = "select count(enbid) as total, emdid
					from emenda.emendadetalheentidade 
				where edestatus = 'A' and emdid = $emdid group by emdid";
		$total = $db->pegaUm($sql, 'total');
		#Atualiza todas entidades para as emendas que possuam somente 1 entidade como recurso e que a liberação seja maior que zero na planilha 1
		if( $valor > 0 && $total == 1 ){			
			foreach ($arEntidade as $entidade) {
				
				$valorReducao = ($entidade['edevalor'] - $valor);
				
				$sql = "UPDATE emenda.emendadetalheentidade SET edevalorreducao = {$valorReducao}, edecarga = 3
						WHERE
							emdid = $emdid
							and edeid = {$entidade['edeid']}
							and edestatus = 'A'
							and edecarga is null";
				$db->executar($sql);
				$edevalorreducao3++;
			}
			$db->commit();
		}
	}
}

$sql = "select ve.edeid, ve.edevalor, eb.enbid, eb.enbcnpj, ve.emecod, ve.gndcod, ve.foncod, ve.mapcod 
		from emenda.v_emendadetalheentidade ve
			inner join emenda.entidadebeneficiada eb on ve.entid = eb.enbid 
		where 
			ve.edestatus = 'A' 
		    and ve.emdimpositiva = 6
		    and ve.emeano = '2014'
		    and eb.enbano = '2014' 
		    and eb.enbstatus = 'A'";
$arrEntBenf = $db->carregar($sql);
$arrEntBenf = $arrEntBenf ? $arrEntBenf : array();

$edevalorreducao4 = 0;

foreach ($arrEntBenf as $v) {
	$boTem = $db->pegaUm("select count(cnpj) from carga.emenda_liberacao_sri_entidade_fnde 
							where replace(replace(replace(replace(btrim(cnpj, ' '), '.', ''), '-', ''), '/', ''), ' ', '') = '{$v['enbcnpj']}'
							and emenda = '{$v['emecod']}'");
	 
	if( (int)$boTem == 0 ){
		$sql = "UPDATE emenda.emendadetalheentidade SET edevalorreducao = {$v['edevalor']}, edecarga = 4
				WHERE
					edeid = {$v['edeid']}
					and edestatus = 'A'
					and edecarga is null";
		$db->executar($sql);
		$edevalorreducao4++;
	}
	$db->commit();
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="center" width="100%" colspan="10"><label class="TituloTela" style="color: rgb(0, 0, 0);">Quantidade de Registro Atualizados</label></td>
	</tr>
	<tr>
		<th>Emenda Detalhe:</th>
		<th>Carga 1:</th>
		<th>Carga 2:</th>
		<th>Carga 3:</th>
		<th>Carga 4:</th>
		<th>Total Emenda Detalhe:</th>
		<th>Valor Carga 1:</th>
		<th>Valor Carga 2:</th>
		<th>Valor Carga 3:</th>
		<th>Valor Carga 4:</th>
	</tr>
	<tr>
		<td><?=$emedaDetalhe; ?></td>
		<td><?=$edevalorreducao1; ?></td>
		<td><?=$edevalorreducao2; ?></td>
		<td><?=$edevalorreducao3; ?></td>
		<td><?=$edevalorreducao4; ?></td>
		<td><?=number_format($db->pegaUm("select sum(ed.emdliberacaosri) from emenda.emendadetalhe ed where ed.emdimpositiva = 6"), 2, ',', '.'); ?></td>
		<td><?=number_format($db->pegaUm("select sum(ede.edevalorreducao) from emenda.emendadetalheentidade ede where edecarga = 1"), 2, ',', '.'); ?></td>
		<td><?=number_format($db->pegaUm("select sum(ede.edevalorreducao) from emenda.emendadetalheentidade ede where edecarga = 2"), 2, ',', '.'); ?></td>
		<td><?=number_format($db->pegaUm("select sum(ede.edevalorreducao) from emenda.emendadetalheentidade ede where edecarga = 3"), 2, ',', '.'); ?></td>
		<td><?=number_format($db->pegaUm("select sum(ede.edevalorreducao) from emenda.emendadetalheentidade ede where edecarga = 4"), 2, ',', '.'); ?></td>
	</tr>
</table>