<?php
$_SESSION['rehuf_var']['entid'] = $_GET['entid'] ? $_GET['entid'] : $_SESSION['rehuf_var']['entid'];
$entid = $_SESSION['rehuf_var']['entid'] ? $_SESSION['rehuf_var']['entid'] : $_GET['entid'];

if($_GET['ent']){
	$_SESSION['rehuf_var']['ent'] = $_GET['ent'] ? $_GET['ent'] : $_SESSION['rehuf_var']['ent'];
	$ent = $_SESSION['rehuf_var']['ent'] ? $_SESSION['rehuf_var']['ent'] : $_GET['ent'];
}

if($_GET['estadoAtual']){
	$_SESSION['rehuf_var']['estadoAtual'] = $_GET['estadoAtual'] ? $_GET['estadoAtual'] : $_SESSION['rehuf_var']['estadoAtual'];
	$estadoAtual = $_SESSION['rehuf_var']['estadoAtual'] ? $_SESSION['rehuf_var']['estadoAtual'] : $_GET['estadoAtual'];
}

if($_GET['prtid']){
	$_SESSION['rehuf_var']['prtid'] = $_GET['prtid'] ? $_GET['prtid'] : $_SESSION['rehuf_var']['prtid'];
	$prtid = $_SESSION['rehuf_var']['prtid'] ? $_SESSION['rehuf_var']['prtid'] : $_GET['prtid'];
}else if($_GET['pltid']){
	$_SESSION['rehuf_var']['pltid'] = $_GET['pltid'] ? $_GET['pltid'] : $_SESSION['rehuf_var']['pltid'];
	$pltid = $_SESSION['rehuf_var']['pltid'] ? $_SESSION['rehuf_var']['pltid'] : $_GET['pltid'];
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/prestacaocontas&acao=A");

if($entid){
	include_once APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc";
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	$e = new Entidade();
		$e->carregarPorId( $entid );
		extract($e->getDados());
}
$arrCabecalho = array('UG','IFES - DESCRIÇÃO','PI','Estado Atual');
if($ent){
	$where = " AND 
				ent.entid = ". $ent;
}
if($estadoAtual){
	$where = " AND 
				edc.esdid = ". $estadoAtual;
}
if($prtid){
	$where = " AND 
				vpli.prtid = ". $prtid;
}else if($pltid){
	$where = " AND 
				vpli.pltid = ". $pltid;
}


	$sql = "
			SELECT 
				'<a href=\'rehuf.php?modulo=principal/prestacaocontas&acao=A\'>'||ent.entungcod||'</a>' as ug,
				ena.entsig ||' - ' || upper(ena.entnome) as ifes,
				pli.plinumero as PI,
				coalesce(edc.esddsc,'Não Iniciado') as estado_atual
			FROM 
				rehuf.prestacaocontas prc
			INNER JOIN
				entidade.entidade ent ON ent.entungcod = prc.prcug
			LEFT JOIN 
				entidade.funcaoentidade fen ON fen.entid = ent.entid 
			INNER JOIN 
				entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN 
				entidade.entidade ena ON ena.entid = fue.entid
			LEFT JOIN 
				rehuf.estruturaunidade esu ON esu.entid = ent.entid
			LEFT JOIN
				rehuf.vinculoplanointerno vpli ON vpli.vpiid = prc.vpiid
			LEFT JOIN
				rehuf.planointerno pli ON pli.pliid = vpli.pliid
			LEFT JOIN
				workflow.documento dc ON dc.docid = vpli.docid
			LEFT JOIN
				workflow.estadodocumento edc ON edc.esdid = dc.esdid
			WHERE 
				fen.funid IN ('16','93') 
			AND 
				(esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)
			
		       {$where} 
			
			GROUP BY ena.entid, ena.entnome, ena.entsig,ent.entunicod,ent.entid,pli.plinumero,edc.esddsc,ent.entungcod";
			
			$db->monta_lista( $sql, $arrCabecalho,  25, 10, 'N', 'center','','');

 ?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function voltar(){
	<?php 
		unset($_SESSION['rehuf_var']['prtid']);
		unset($_SESSION['rehuf_var']['pltid']);
		unset($_SESSION['rehuf_var']['ent']);
		unset($_SESSION['rehuf_var']['estadoAtual']);
	 ?>
	var url = "rehuf.php?modulo=relatorio/relatorioPrestacaoContas&acao=A";
		jQuery(location).attr('href',url);
	}
function salvarPrestacaoContas(){
			jQuery("#formulario").submit();
	
}
</script>
<form method="POST" id="formulario" name="formulario">
	 <table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
		 	<tr>
				<td width="25%" class="SubTituloDireita"></td>
				<td>
		 			<input type="button" name="btn_voltar" value="Voltar" onclick="voltar()" />
		 		</td>
		 	</tr>
	</table>
</form>
			