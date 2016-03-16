<?php
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco(); 

	function retornaMunicipio($campo, $valor) {
		global $db;
		$sql = "select estuf||muncod as obj from territorios.municipio where ".$campo."='".$valor."'";
		$arrMuncod = $db->carregarColuna($sql);
		return implode(",", $arrMuncod);
	}
	
	$sql = "select 
				mun.*,
				estdescricao,
				( select sum(distinct munpopulacao) from territorios.municipio m3 where m3.estuf = mun.estuf  ) as populacao_est,
				mun.mescod,
				mesdsc,
				( select sum(distinct munpopulacao) from territorios.municipio m1 where m1.mescod = mun.mescod  ) as populacao_mes,
				mun.miccod,
				mic.micdsc,
				( select sum(distinct munpopulacao) from territorios.municipio m2 where m2.miccod = mun.miccod  ) as populacao_mic,
				( select sum(distinct munpopulacao) from territorios.municipio m4 inner join territorios.municipiosvizinhos mv on m4.muncod = mv.muncodvizinho where mv.muncod = mun.muncod ) as populacao_viz
			from 
				territorios.municipio mun
			inner join
				territorios.estado e on e.estuf = mun.estuf
			inner join
				 territorios.mesoregiao mes ON mes.mescod = mun.mescod
			inner join
				 territorios.microregiao mic ON mic.miccod = mun.miccod
			where 
				mun.muncod = '{$_POST['muncod']}' ";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados)
		extract($arrDados);
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script>
		function exibeListaMunicipio(muncod){
		window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	}
	</script>
	<div style="font-size:12px;margin-3px;height:350px;overflow:auto;text-align:center;" >
	<b>Localização:</b> <span style="cursor:pointer" onmouseover="nomePoli['<?=$estuf.$muncod?>'].setOptions( {fillColor: '#d82b40'} );" onmouseout="f_mudacor('<?=$estuf.$muncod?>', corPoli['<?=$estuf.$muncod?>']);" onclick="window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun=<?php echo $mundescricao ?>&codmun=<?php echo substr($muncod,0,6) ?>&r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no')" > <?php echo $mundescricao ?> / <?php echo $estuf ?> </span>
	<table style="margin-top:4px" class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" >
		<thead>
			<tr><td>&nbsp;</td><td>Tipo</td><td>População</td><td>% Estado</td></tr>
		</thead>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=$muncod ?>',this);"></td><td>Município:</td><td align="right"><?php echo number_format($munpopulacao,0,".",".") ?></td><td align="right"><?php echo round(( $munpopulacao/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=$muncod ?>',this);"></td><td>Municípios Vizinhos:</td><td align="right"><?php echo number_format($populacao_viz,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_viz/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=retornaMunicipio("miccod",$miccod) ?>',this);"></td><td>Microregião: <?php echo $micdsc ?></td><td align="right"><?php echo number_format($populacao_mic,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_mic/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="mesoregiao" onclick="f_mudacores('<?=retornaMunicipio("mescod",$mescod) ?>',this);"></td><td>Mesoregião: <?php echo $mesdsc ?></td><td align="right"><?php echo number_format($populacao_mes,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_mes/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="estado" onclick="f_mudacores('<?=retornaMunicipio("estuf",$estuf) ?>',this);"></td><td>Estado: <?php echo $estdescricao ?></td><td align="right"><?php echo number_format($populacao_est,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_est/$populacao_est )*100,2) ?>%</td></tr>
	</table>
	<br />
	<table class="tabela" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" >
		<tr><td class="SubtituloEsquerda" ><b>Outras Classificações</b></td></tr>
	</table>
	
	<?php $sql = "select gtmdsc,tpmdsc from 
					territorios.muntipomunicipio  m
				inner join
					territorios.tipomunicipio t ON t.tpmid = m.tpmid
				inner join
					territorios.grupotipomunicipio g ON g.gtmid = t.gtmid
				where 
					muncod = '{$_POST['muncod']}'";
	$cabecalho = array("Grupo","Tipo");
	$db->monta_lista_simples($sql,$cabecalho,10,10,"N","95%","N");?>
	<br />
	</div><br />
	<div style="font-size:12px;cursor:pointer" onclick="exibeListaMunicipio('<?php echo $_POST['muncod'] ?>');" style="cursor:pointer;width:100%;text-align:right">Mais detalhes...</div>
	
