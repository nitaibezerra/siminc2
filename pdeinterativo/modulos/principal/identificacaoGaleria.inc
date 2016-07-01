<?php
monta_titulo( "Galeria de Fotos", '&nbsp' );
$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
?>

<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="25%" class="SubTituloDireita blue" style="height:50px;">Orientações</td>
		<td class="SubTituloEsquerda blue normal" style="background-color:#f5f5f5">Clique na imagem para vê-la em tamanho original.</td>
	</tr>
<?php 
$sql = "SELECT 
			aefid,
			aefdesc,
			coalesce(aefqtdfotoobrigatoria, 0) AS aefqtdfotoobrigatoria,
			coalesce(aefqtdfoto, 0) AS aefqtdfoto
		FROM 
			pdeinterativo.ambienteescolafoto 
		WHERE 
			aefstatus = 'A'";
$dadosambienteescola = $db->carregar($sql);

if($dadosambienteescola)
{
	foreach($dadosambienteescola as $ambienteescola)
	{
?>
<tr>
	<td width="25%" class="SubTituloDireita" ><?=$ambienteescola['aefdesc']?></td>
	<td class="SubTituloEsquerda normal" style="background-color:#f5f5f5">
	<div style="float:left;width:100%;padding-bottom:10px;">
	<?php
	$sql = "SELECT 
				* 
			FROM 
				pdeinterativo.galeriafoto 
			WHERE 
				gfostatus = 'A' 
				AND aefid = ".$ambienteescola['aefid']." 
				AND pdeid = ".$arrDados["pdeid"]."";
	$dadosgaleriafoto = $db->carregar($sql);

	$_SESSION['downloadfiles']['pasta'] = array("origem" => "pdeinterativo","destino" => "pdeinterativo");
	
	$_SESSION['imgparams'] = array("filtro" => "gfostatus = 'A' AND aefid = ".$ambienteescola['aefid']." AND pdeid = ".$arrDados["pdeid"]."", "tabela" => "pdeinterativo.galeriafoto");
	
	if($dadosgaleriafoto)
	{
		foreach($dadosgaleriafoto as $galeriafoto)
		{
			echo "<div style=\"float:left; text-align:center; margin:3px;\" >
				  	<img border='1px' id='".$galeriafoto["arqid"]."' src='../slideshow/slideshow/verimagem.php?arqid=".$galeriafoto["arqid"]."&newwidth=70&newheight=70' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>
				  	<br />
				  	<img style=\"cursor:pointer; position:relative; z-index:10; top:-75px; left:-10px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"excluirFoto(".$galeriafoto["arqid"].");\">
				  </div>";
		}
	} 
	else
	{
		echo "<font style=\"color:red\">* Nenhum arquivo encontrado</font>";
	}
	?>
	</div>
	<input type="button" value="Inserir nova foto" id="<?=($ambienteescola['aefid'])?>" class="btNovaFoto" />
	<br />
	Quantidade de fotos a ser inserida: Mínima<input type="text" style="width:20px;" value="<?=($ambienteescola['aefqtdfotoobrigatoria'])?>" disabled="disabled" />&nbsp;&nbsp;&nbsp;Máxima<input type="text" style="width:20px;" value="<?=($ambienteescola['aefqtdfoto'])?>" disabled="disabled" />
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" >O que deseja fazer agora?</td>
	<td>
		<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola'" >
		<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A'" >
	</td>
</tr>
<?php
	}
}
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery(document).ready(function($)
{
	$(window).unload(function()
	{
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/identificacao&acao=A",
			   async: false,
			   dataType: "json",
			   data: "requisicaoAjax=verificaQtdMinima&pdeid=<?=$arrDados["pdeid"]?>",
			   success: function(data)
			   {
				 if( data.length > 0 )
				 {
					 var msgAlert = '';
					 
				   	 $.each(data, function(key, val)
					 {
				   		msgAlert = msgAlert + '\n- ' + val;
					 });

					 alert("O mínimo necessário de fotos não foi alcançado no(s) seguinte(s) ambiente(s):"+msgAlert);
				 }
			   }
		});
	});
	
	$('.btNovaFoto').click(function()
	{
		if( verificaQtdMaxima( $(this).attr('id') ) )
		{
			var janela = window.open('pdeinterativo.php?modulo=principal/popIncluirFoto&acao=A&pdeid=<?=$arrDados["pdeid"]?>&aefid='+$(this).attr("id"), 'popIncluirFoto', 'width=500,height=280,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
			janela.focus();
		}
		else
		{
			alert('A quantidade de fotos inserida já está no limite máximo permitido.');
		}
	});

	function verificaQtdMaxima(aefid)
	{
		var retorno;
		
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/identificacao&acao=A",
			   async: false,
			   data: "requisicaoAjax=verificaQtdMaxima&pdeid=<?=$arrDados["pdeid"]?>&aefid="+aefid,
			   success: function(msg)
			   {
			   	 if( msg == 'ok' )
			   		retorno = true;
			   	 else
			   		retorno = false;
			   }
		});

		return retorno;
	}

	
});

function excluirFoto(arqid)
{
	if( confirm("Deseja realmente excluir o documento anexado?") )
	{
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/identificacao&acao=A",
			   async: false,
			   data: "requisicaoAjax=excluirFoto&arqid="+arqid,
			   success: function(msg)
			   {
				if( msg == 'ok' )
			   	 {
					alert('Foto excluída com sucesso.');
					window.location.href = window.location.href;
			   	 }
			   	 else
			   	 {
			   		alert('Ocorreu um erro ao excluir a foto.');
			   		return;
			   	 }
			   }
		});
	}
}

</script>