<?php
ini_set("memory_limit","1000M");
set_time_limit(0);

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

$oPreObra 			= new PreObra();
$oSubacaoControle 	= new SubacaoControle();
global $db;

$inuid = $_SESSION['par']['inuid'];

if( !$inuid || $inuid == '' || empty($_REQUEST['terid']) ){
	echo "<script>alert('Entidade não encontrada!');window.close();</script>";
	exit;
}

if( $_REQUEST['envioConcluido'] ){
	$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = ".$_REQUEST['terid']." AND inuid = ".$inuid );
	if( $arqid ){
		$file = new FilesSimec();
		$arquivo = $file->getDownloadArquivo($arqid);
		//echo"<script>window.location.href = 'par.php?modulo=principal/termoAnalise&acao=A';</script>";
		exit;
	}
}

$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = ".$_REQUEST['terid']." AND inuid = ".$inuid );
if( $arqid ){
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	//echo"<script>window.location.href = 'par.php?modulo=principal/termoAnalise&acao=A';</script>";
	exit;
}

?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css" />
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	function enviarParaAnalise(){
		carregaTela();
		document.getElementById('salva').value = 'envia';
		document.getElementById('formulario').submit();
	}
	
	function carregaTela(id){
		var d = document;
		var div, span, img, h, w, topImg, elementRefe;
		//var j = jQuery.noConflict();
		id = id ? id : '';
		h = d.body.scrollHeight;
		w = d.body.scrollWidth;
		
		elementRefe = typeof(id) != 'object' ? d.getElementById(id) : id;
		h = h < screen.height ? screen.height : h;
		
		if (elementRefe){
			topImg = findPosY(elementRefe);
		}else{
			topImg = (h/4);
		}
		
		if (!jQuery("#temporizador1")[0]){
			div = d.createElement("div");
		}else{
			div = jQuery("#temporizador1");		
			jQuery(div).remove(div);
		}
		
		// Monta Span
		if (jQuery("span", div).length == 0){
			span = d.createElement("span");
		}else{
			span = jQuery("span", div).eq(0);
			jQuery("span", div).remove();
		}
		
		jQuery(span).attr({
			id : 'spanCarregando'
		})
		.css({
			'position' : 'relative',
			'top'	   : topImg + 'px'
		})
		.append('<center>Carregando...</center>');
		
		// Monta Imagem
		if (jQuery("img", span).length == 0){
			img = d.createElement("img");
		}else{
			img = jQuery("img", span).eq(0);
			jQuery("img", span).eq(0).remove();
		}
		
		jQuery(img).attr({
			src : '/imagens/carregando.gif'
		});
		jQuery("center", span).before(img);
		
		// Insere span com img na div, e prepara a mesma
		jQuery(div).append(span)
		.attr({
			id : 'temporizador1'			
		})
		.css({
			'-moz-opacity' : '0.8', 
			'filter' 	   : 'alpha(opacity=80)', 
			'background'   : '#ffffff',
			'text-align'   : 'center',
			'position' 	   : 'absolute', 
			'top' 		   : '0px', 
			'left' 		   : '0px', 
			'width'		   : w + 'px', 
			'height' 	   : h + 'px', 
			'z-index' 	   : '1000'		
		});
			
		// Insere no nó a div	
		jQuery(d.body).append(div);
		
		return;
		//return div;
		
		function findPosY(obj){
			var curtop = 0;
		    if(obj.offsetParent)
		        while(1)
		        {
		          curtop += obj.offsetTop;
		          if(!obj.offsetParent)
		            break;
		          obj = obj.offsetParent;
		        }
		    else if(obj.y)
		        curtop += obj.y;
		    return curtop;
		}
		
	}
</script>
<?php
if( $_POST['salva'] == 'envia' ){
	//envia para a analise!
	if( $_REQUEST['terid'] == 1 ){
		$_SESSION['par']['pendenciaBP'] = true;
		$itrid 			= 3; 
	} else {
		$itrid 			= $_SESSION['par']['itrid']; 
	}
	$envio = enviarParaAnalisePar($itrid, $inuid, $oPreObra, $oSubacaoControle);

	if( $envio == 'pendencia' ){
		echo '<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<input type="hidden" name="salva" id="salva" value="">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Você possui pendências de preenchimento. Finalize o PAR antes de enviá-lo.</strong></td>
				</tr>
			</table>';
			include "pendenciaDePreenchimento.inc";
			unset($_SESSION['par']['pendenciaBP']);
		exit;

	} elseif( $envio == true ) {
		
		echo '<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<input type="hidden" name="salva" id="salva" value="">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Envio Concluído.</strong></td>
				</tr>
			</table>';
		echo"<script>window.location.href = 'par.php?modulo=principal/termoAnalise&acao=A&envioConcluido=1';window.opener.document.location.reload();</script>";
		exit;

	} else {
		echo '<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<input type="hidden" name="salva" id="salva" value="">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Problema na execução.</strong></td>
				</tr>
			</table>';
		exit;

	}
}

$sql = "SELECT * FROM par.termo WHERE terid = ".$_REQUEST['terid'];
$dados = $db->pegaLinha( $sql );

if( $_SESSION['par']['itrid'] == 2 ){
	// PREFEITO
	$sql = "SELECT
				ent.entnome as nome, 
				ent.entnumcpfcnpj as cnpj
			FROM 
				par.entidade ent 
			INNER JOIN par.instrumentounidade iu ON iu.inuid = ent.inuid
			WHERE
				ent.entstatus = 'A' AND
				ent.dutid = ".DUTID_PREFEITO." AND
				iu.muncod = '".$_SESSION['par']['muncod']."'";
	/*
	$sql = "select 
				entnome as nome, 
				entnumcpfcnpj as cnpj
			from 
				entidade.entidade 
			where 
				entid = (SELECT
							max(ent.entid) as entid1				
						FROM 
							entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 1 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
						WHERE 
							(ent.entstatus = 'A' OR ent.entstatus IS NULL)
							and eed2.muncod = '".$_SESSION['par']['muncod']."'and eed2.estuf = '".$_SESSION['par']['estuf']."')";
	*/
} else {
	// SECRETARIA
	$sql = "SELECT
				ent.entnome as nome, 
				ent.entnumcpfcnpj as cnpj
			FROM 
				par.entidade ent 
			WHERE
				ent.entstatus = 'A' AND
				ent.dutid = ".DUTID_SECRETARIA_ESTADUAL." AND 
				ent.estuf = '".$_SESSION['par']['estuf']."'";
	/*
	$sql = "select 
				entnome as nome, 
				entnumcpfcnpj as cnpj
			from 
				entidade.entidade 
			where 
				entid = (SELECT
							max(ent.entid) as entid1				
						FROM entidade.entidade ent
							INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
							INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 6 AND fue.fuestatus = 'A'
							INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
						WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
						and eed2.estuf = '".$_SESSION['par']['estuf']."')";
	*/
	
}

$dadosEnt = $db->pegaLinha( $sql );

$conteudo = str_replace('{razaosocial}', $dadosEnt['nome'], $dados['terconteudo']);
$conteudo = str_replace('{cnpj}', $dadosEnt['cnpj'], $conteudo);
$conteudo = str_replace('{anoinicio}', date('Y'), $conteudo);

echo '<form name="formulario" id="formulario" method="post" action="" >
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<input type="hidden" name="salva" id="salva" value="">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>'.$dados['terdescricao'].'</strong></td>
				</tr>
				<tr><td>';

echo $conteudo;

echo '</td></tr><tr><td class="SubTituloEsquerda" >
			<input type="button" value="Concordo" name="btn_enviar" onclick="enviarParaAnalise()" />
			<input type="button" value="Fechar" name="btn_fechar" onclick="window.close();" />
		</td>';
echo '</table></form>';


