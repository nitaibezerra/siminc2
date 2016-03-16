<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "../_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct
			co_sub_item_projeto,
		    co_natureza_despesa,
		    ds_sub_item_projeto
		from(
		    SELECT 
		        co_sub_item_projeto,
		        co_natureza_despesa,
		        ds_sub_item_projeto
		    FROM 
		        carga.emenda_item_natureza
		    WHERE
		        co_sub_item_projeto is not null
		    union all
		    SELECT
		        subitem as co_sub_item_projeto,
		        acao_orcamentaria as co_natureza_despesa,
		        descricao as ds_sub_item_projeto
		    FROM 
		        carga.emenda_especificacao_siconv ees
		    where
		        ees.codigo not in (
		    SELECT 
		        codigo
		    FROM 
		        carga.emenda_especificacao_siconv ees
		        inner join emenda.siconvsubitens ssi on ssi.ssicodigosubitem = ees.subitem
		    )
		) as foo";

$arrItemNat = $db->carregar($sql);
$arrItemNat = $arrItemNat ? $arrItemNat : array();

foreach ($arrItemNat as $v) {
	$sql = "select count(ssi.ssiid) 
			from emenda.siconvsubitens ssi
				inner join emenda.siconvnaturezadespesa snd on snd.sndid = ssi.sndid
			where
				ssi.ssicodigosubitem = '{$v['co_sub_item_projeto']}'
			    and snd.sndcodigonaturezadespesa = '{$v['co_natureza_despesa']}'";
	$boTem = $db->pegaUm($sql);
	
	if( (int)$boTem == 0 ){
		$sndid = $db->pegaUm("SELECT sndid FROM emenda.siconvnaturezadespesa WHERE sndcodigonaturezadespesa = '{$v['co_natureza_despesa']}'");
		
		$sql = "INSERT INTO emenda.siconvsubitens(ssiid, ssicodigosubitem, ssidescricaosubitem, sndid) 
				VALUES ( (select max(ssiid) + 1 from emenda.siconvsubitens), '{$v['co_sub_item_projeto']}', '{$v['ds_sub_item_projeto']}', $sndid)";
		$db->executar($sql);
		$db->commit();
	}
}

?>