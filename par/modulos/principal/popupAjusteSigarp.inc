<?php 
$id = $_GET['id'];

if( $_POST['requisicao'] == 'salvar' ){
	
	global $db;
	
	$sql = "SELECT * FROM carga.adesoesnaoencontradas6 WHERE id = ".$_REQUEST['id'];
	$dados = $db->pegaLinha($sql);
	
	$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$_POST['sbdid'];
	$dadosSubacao = $db->pegaLinha($sql);

	$sql = "SELECT pic.idsigarp FROM par.subacaoitenscomposicao sic INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid WHERE icoid = ".$_POST['item'];
	$itemsigarp = $db->pegaUm($sql);
	
	$sql = "INSERT INTO par.empenhopregaoitensenviados ( prpid, sbaid, idsigarp, adesao, usucpf, epistatus, epiano, epicarga ) 
			VALUES ( ".$_POST['processo'].", ".$dadosSubacao['sbaid'].", ".$itemsigarp.", ".$dados['adesao'].", '".$_SESSION['usucpf']."', 'A', ".$dadosSubacao['sbdano'].", 't' ) RETURNING epiid";

	$epiid = $db->pegaUm($sql);
	
	$sql = "select 
				icoid, 
				sbdid
			from 
				par.subacaoitenscomposicao sic 
			inner join par.subacaodetalhe sd on sd.sbaid = sic.sbaid and sd.sbdano = sic.icoano 
			inner join par.propostaitemcomposicao pic on pic.picid = sic.picid
			where 
				sic.sbaid = ".$dadosSubacao['sbaid']." and 
				pic.idsigarp = ".$itemsigarp;
	
	$dadosEmpenho = $db->pegaLinha($sql);
	
	$sql = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$_POST['processo']." AND sbdid = ".$dadosEmpenho['sbdid']." AND icoid = ".$dadosEmpenho['icoid'];
	$db->executar($sql);
	
	$sql = "UPDATE carga.adesoesnaoencontradas6 SET epiid = ".$epiid." WHERE id = ".$_REQUEST['id'];
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload(); 
				window.close();
			</script>";
	exit();
}

if( $_REQUEST['carregarSubacao'] ){
	
	$sql = "SELECT 
				sd.sbdid as codigo, 
				par.retornacodigosubacao(s.sbaid) || ' - ' || s.sbadsc as descricao 
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			WHERE 
				ppc.prpid = ".$_REQUEST['prpid']."
				and ppc.ppcstatus = 'A'
			ORDER BY
				descricao ";

	return $db->monta_combo( "sbdid", $sql, 'S', 'Selecione a Subação', 'carregaItens(this.value);', '', '', 500 );
}

if( $_REQUEST['carregarItens'] ){
	
	global $db;
	
	$sql = "SELECT 
				p.inuid,
				sd.sbaid,
				sd.sbdano,
				sic.icoid as codigo, 
				pic.picdescricao as descricao,
				CASE WHEN s.sbacronograma = 1 
					THEN sum(coalesce(sic.icoquantidadetecnico,0))
				ELSE 
					CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						THEN -- escolas sem itens
							sum(coalesce(se.sesquantidadetecnico,0))
						ELSE -- escolas com itens
							CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
								sum(coalesce(ssi.seiqtdtecnico,0))
							ELSE
								sum(coalesce(ssi.seiqtd,0))
							END
						END
				END AS quantidade
			FROM 
				par.subacaodetalhe sd
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND pic.idsigarp IS NOT NULL
			LEFT JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano 
			LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
			WHERE 
				sd.sbdid = ".$_REQUEST['sbdid']."
				AND (sd.sbaid, sd.sbdano, idsigarp) NOT IN ( SELECT sbaid, epiano, idsigarp FROM par.empenhopregaoitensenviados WHERE epistatus = 'A' )
			GROUP BY
				sic.icoid,
				pic.picdescricao,
				s.sbacronograma,
				s.frmid, 
				s.ptsid,
				sic.icovalidatecnico,
				p.inuid,
				sd.sbaid,
				sd.sbdano
			ORDER BY
				descricao";
	$itens = $db->carregar($sql);

	if( is_array($itens) ){
		$it = "<table class=\"tabela\" border=\"1\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
					<tr>
						<th>Ação</th>
						<th>Item</th>
						<th>Quantidade</th>
					</tr>";
		foreach( $itens as $item ){
			$it .= "<tr>
						<td nowrap><img src=\"/imagens/icone_lupa.png\" title=\"Visualizar Subação\" onclick=\"javascript: listarSubacao(".$item['sbaid'].",".$item['sbdano'].",".$item['inuid'].")\">&nbsp;
							<input type=\"radio\" class=\"classeradio\" id=\"item\" name=\"item\" value=\"".$item['codigo']."\">
							<input type=\"hidden\" id=\"item_".$item['codigo']."\" name=\"item_".$item['codigo']."\" value=\"".$item['descricao']."\">
							</td>
						<td>".$item['descricao']."</td>
						<td>".$item['quantidade']."</td>
					</tr>";
		}
		$it .= "</table>";
		echo $it;
		die();
	}
	
}

?>
<html>
	<head>
		<title>PAR - Ajustes SIGARP</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript">
			function selecionaprocesso(prpid)
			{
				if(prpid){
					jQuery( '#prpidHidden' ).val( prpid );
						jQuery.ajax({
							  type		: 'post',
							  url		: window.location,
							  data		: 'carregarSubacao=1&prpid='+prpid,
							  success	: function(res) {
							  		jQuery( '#itens' ).html('');
							  		jQuery( '#subacao' ).html( res );
							  }
						});
				}
			}
			function carregaItens(sbdid)
			{
				if(sbdid){
					jQuery( '#sbdidHidden' ).val( sbdid );
						jQuery.ajax({
							  type		: 'post',
							  url		: window.location,
							  data		: 'carregarItens=1&sbdid='+sbdid,
							  success	: function(res) {
							  		jQuery( '#itens' ).html( res );
							  }
						});
				}
			}

			function listarSubacao(sbaid, ano, inuid){
				var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
				janela(local,800,600,"Subação");
			}
			
			function salvar(){
				if($('.classeradio:checked').length == 0){
					alert('Você precisa selecionar um item.');
				} else {	
					item = $('.classeradio:checked').val();
					descricao = $('#item_'+item).val();
					itemsigarp = $('#descricaoitem').val();
					if( confirm("Tem certeza que quer vincular o item "+itemsigarp+" ao item "+descricao+"?") ){
						$('#requisicao').val('salvar');
						$('#formsigarp').submit();
					}
				}
			}
		</script>	
	</head>
	<body>
	<?php	
	
	global $db;
	
	monta_titulo( 'Ajustes SIGARP', ''  );
	
	$sql = "SELECT item FROM carga.adesoesnaoencontradas6 WHERE id = ".$id;
	$descricao = $db->pegaUm($sql);
	
	$sql = "SELECT DISTINCT
				prp.prpid as codigo,
				prp.prpnumeroprocesso as descricao
			FROM
				carga.adesoesnaoencontradas6 ane
			INNER JOIN par.instrumentounidade iu ON iu.inucnpj = ane.cnpj
			LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
			INNER JOIN par.processopar prp ON prp.inuid = iu.inuid AND prpstatus = 'A'
			INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			WHERE
				id = ".$id;
	?>
	<form id="formsigarp" action="<? echo $_SERVER['REQUEST_URI'] ?>" method="post">
		<input type="hidden" name="prpidHidden" id="prpidHidden" value="" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />
		<input type="hidden" name="descricaoitem" id="descricaoitem" value="<?=$descricao ?>" />
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td width="150px" class="SubTituloDireita">Processo:</td>
				<td><?php $db->monta_combo('processo',$sql,'S','Selecione...','selecionaprocesso','','','200','S','','',$processo); ?></td>
			</tr>
			<tr>
				<td width="150px" class="SubTituloDireita">Subação:</td>
				<td id="subacao"></td>
			</tr>
			<tr>
				<td width="150px" class="SubTituloDireita">Item(ns):</td>
				<td id="itens"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"></td>
				<td><input type="button" value="Salvar" onclick="salvar()" /></td>
			</tr>			
		</table>
	</form>
	</body>
</html>