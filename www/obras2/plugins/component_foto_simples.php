<?php
include "conf_fotos.php";
include "config.inc";

include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."www/obras2/_funcoes.php";
include APPRAIZ."www/obras2/_constantes.php";

include_once APPRAIZ . "www/autoload.php";

$db = new cls_banco();

function deletarfotos($dados) {
	session_start();
	global $db;
	if($_SESSION['obras2']['supid']) {
		$fotid = $db->pegaUm("SELECT fotid 
                                      FROM obras2.fotos
                                      WHERE obrid='".$_SESSION['obras2']['obrid']."' 
                                        AND supid='".$_SESSION['obras2']['supid']."'                         
                                        AND fotordem='".($dados['fotordem']-1)."'");
		
		if($fotid) {
                    $db->executar("DELETE FROM obras2.fotos WHERE fotid='".$fotid."'");
                    $db->executar("UPDATE obras2.fotos SET fotordem=(fotordem-1),fotbox='imageBox'||(fotordem-1) WHERE fotordem > ".($dados['fotordem']-1)." AND obrid='".$_SESSION['obras2']['obrid']."' AND supid='".$_SESSION['obras2']['supid']."'");
                    $db->commit();
		}

	}
	if($_SESSION['obras2']['emiid']) {
		$fotid = $db->pegaUm("SELECT fotid FROM obras2.fotos WHERE obrid='".$_SESSION['obras2']['obrid']."' AND emiid='".$_SESSION['obras2']['emiid']."' AND fotordem='".($dados['fotordem']-1)."'");
		
		if($fotid) {
                    $db->executar("DELETE FROM obras2.fotos WHERE fotid='".$fotid."'");
                    $db->executar("UPDATE obras2.fotos SET fotordem=(fotordem-1),fotbox='imageBox'||(fotordem-1) WHERE fotordem > ".($dados['fotordem']-1)." AND obrid='".$_SESSION['obras2']['obrid']."' AND emiid='".$_SESSION['obras2']['emiid']."'");
                    $db->commit();
		}

	}
	exit;
}


function ordenarfotos($dados) {
	session_start();
	global $db;
        
	if($_SESSION['obras2']['supid']) {
		$fotatual = $db->pegaUm("SELECT fotid FROM obras2.fotos WHERE obrid='".$_SESSION['obras2']['obrid']."' AND supid='".$_SESSION['obras2']['supid']."' AND fotordem='".($dados['ordematual']-1)."'");
		$fotir = $db->pegaUm("SELECT fotid FROM obras2.fotos WHERE obrid='".$_SESSION['obras2']['obrid']."' AND supid='".$_SESSION['obras2']['supid']."' AND fotordem='".($dados['ordemir']-1)."'");
		if($fotatual && $fotir) {
                    $db->executar("UPDATE obras2.fotos SET fotordem='".($dados['ordemir']-1)."', fotbox='imageBox".($dados['ordemir']-1)."' WHERE fotid='".$fotatual."'");
                    $db->executar("UPDATE obras2.fotos SET fotordem='".($dados['ordematual']-1)."', fotbox='imageBox".($dados['ordematual']-1)."' WHERE fotid='".$fotir."'");
                    $db->commit();
		}
	}
        
	if($_SESSION['obras2']['emiid']) {
		$fotatual = $db->pegaUm("SELECT fotid FROM obras2.fotos WHERE obrid='".$_SESSION['obras2']['obrid']."' AND emiid='".$_SESSION['obras2']['emiid']."' AND fotordem='".($dados['ordematual']-1)."'");
		$fotir = $db->pegaUm("SELECT fotid FROM obras2.fotos WHERE obrid='".$_SESSION['obras2']['obrid']."' AND emiid='".$_SESSION['obras2']['emiid']."' AND fotordem='".($dados['ordemir']-1)."'");
		if($fotatual && $fotir) {
                    $db->executar("UPDATE obras2.fotos SET fotordem='".($dados['ordemir']-1)."', fotbox='imageBox".($dados['ordemir']-1)."' WHERE fotid='".$fotatual."'");
                    $db->executar("UPDATE obras2.fotos SET fotordem='".($dados['ordematual']-1)."', fotbox='imageBox".($dados['ordematual']-1)."' WHERE fotid='".$fotir."'");
                    $db->commit();
		}
	}
	exit;
}

if( !empty($_REQUEST['requisicao']) ) {
	$_REQUEST['requisicao']($_REQUEST);
}

header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header('Content-Type: text/html; charset=iso-8859-1');

$arPerfil = array(PFLCOD_SUPERVISOR_MEC, 
                  PFLCOD_ADMINISTRADOR, 
                  PFLCOD_SUPERVISOR_UNIDADE, 
                  PFLCOD_EMPRESA_VISTORIADORA_FISCAL, 
                  PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                  PFLCOD_GESTOR_UNIDADE,
                  PFLCOD_GESTOR_MEC,
                  PFLCOD_SUPER_USUARIO
                 );
				  
if ( possuiPerfil( $arPerfil ) ){
	$html = ""; // botão de enviar arquivos
	$adicionar_novos = '<a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a><br />';
}else{
	$html = " disabled=\"disabled\" ";
	$adicionar_novos = '<br />';
}

?>
<html>
<head>
	<meta http-equiv="Pragma" content="no-cache">
	<title>Inserir fotos</title>
	<link href="css/default.css" rel="stylesheet" type="text/css" />
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	<script type="text/javascript" src="js/utils.js"></script>
	<script type="text/javascript" src="../../includes/prototype.js"></script>
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
jQuery.noConflict();

var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;

function verificaPreenchimento(){
    for(x = 0; x < jQuery('select').length; x++){
        if (jQuery('select').eq(x).val() == '0') {
            alert('Selecione um etapa para cada foto.');
            return false;
        }
    }
}

function ajaxatualizar(params,iddestinatario) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
				}
			}
		});
}

function excluirfotosvistoria(indiceselecionado,id) {
	var tabela = document.getElementById('listaimagens');
	tabela.deleteRow(indiceselecionado);
	if(tabela.rows.length == 2) {
		document.getElementById('submit1').disabled = true;
		document.getElementById('submit2').disabled = true;
	}
	jQuery('#foto_' + id,window.opener.document).remove();
	ajaxatualizar("fotordem="+indiceselecionado+"&requisicao=deletarfotos");
}


function ordenar(indiceselecionado, acao) {
	switch(acao) {
		case 'subir':
			if(indiceselecionado > 1) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado-1)];
				var desc2 = tabela.rows[(indiceselecionado-1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado-1)+"&requisicao=ordenarfotos");
				atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
			}
			break;
		case 'descer':
			var tabela = document.getElementById('listaimagens');
			if((tabela.rows.length-2) > indiceselecionado) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado+1)];
				var desc2 = tabela.rows[(indiceselecionado+1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado+1)+"&requisicao=ordenarfotos");
				atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
			}
			break;
	} 	
}

function validarInsercaoFiles() {
	var anexovazio = true;
	for(i=0;i<document.anexo.elements.length;i++) {
            if(document.anexo.elements[i].type == "file") {
                if(document.anexo.elements[i].value != "") {
                        anexovazio = false;
                }
            }
	}
	if(anexovazio) {
            alert("Não existem arquivos anexados.");
            return false;
	} else {
            return true;
	}
}

function inserirNovosArquivos() {
	var tabela = document.getElementById('inserirarquivos');
    if(jQuery('#inserirarquivos input[type=file]').length == 30){
        alert('Somente é permitido o envio de 30 arquivos por vez.');
        return;
    }
	for(i=0;i<2;i++) {
            var line = tabela.insertRow((tabela.rows.length-2));
            var cell = line.insertCell(0);
            cell.innerHTML = "<input type=\"file\" name=\"Filedata[]\">";
	}
}

function isNumber(numero){
	var CaractereInvalido = false;
	for (i=0; i < numero.length; i++) {
            var Caractere = numero.charAt(i);
            if(Caractere != "." && Caractere != "," && Caractere != "-"){
                if (isNaN(parseInt(Caractere))) CaractereInvalido = true;
            }
	}
	return !CaractereInvalido;
}

function inserirobservacaothumnails(id, descricao) {
/*	var opener_container = window.opener.document.getElementById("thumbnails");
//	for(var k=0;k<opener_container.childNodes.length;k++){
//		var child = opener_container.childNodes;
//		if(child[k].firstChild){
//			var imagem = child[k].firstChild;
//			if(!isNumber(imagem.id)) {
//				var img  = ""+imagem.id+"";
//				if(img == id) {
//					imagem.title = descricao
//				}
//			}
//		}
//
//	}
*/
}

function inserirfotosthumbnailsPorNome(nome){
    var num = jQuery('#fotos_supervisao li',window.opener.document);
    jQuery('#fotos_supervisao',window.opener.document).append( '<li class="nodraggable" id="foto_'+ nome + '"><img width="96" height="76.65" src="./plugins/resize.php?img=../../../arquivos/obras2/imgs_tmp/' + nome + '&w=85&h=100" class=\"img_foto\"></li>' );
}

function inserirfotosthumbnails(id) {	
	var num = jQuery('#fotos_supervisao li',window.opener.document);	
	var pag = parseInt(num.size() / 16);
	jQuery('#fotos_supervisao',window.opener.document).append( '<li class="draggable" id="foto_'+ id + '"><img width="96" height="76.65" src="../slideshow/slideshow/verimagem.php?arqid=' + id + '&newwidth=100&newheight=85" ondblclick="abrirGaleria(\'829800\',\'' + pag + '\')" class=\"img_foto\"></li>' );
}

function atualizaFotosAjax(params,iddestinatario){
	var myAjax = new Ajax.Request(
		window.opener.location.href,
		{
                    method: 'post',
                    parameters: params,
                    asynchronous: false,
                    onComplete: function(resp) {
                        if(iddestinatario) {
                                iddestinatario.innerHTML = resp.responseText;
                        } 
                    },
                    onLoading: function(){
                        if(iddestinatario) {
                                iddestinatario.innerHTML = 'Carregando...';
                        }
                    }
		});
}
	
</script>
</head>
<iframe name="iframeUpload" style="width:100%;height:400px;display:none;"></iframe>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<br />
<form accept-charset="utf-8" method="post" id="anexo" name="anexo" enctype="multipart/form-data" action="upload_simples.php?funcao=<? echo $_REQUEST['funcao']; ?>" onsubmit="return validarInsercaoFiles();" target="iframeUpload">
<? //echo montarAbasArray_($abas, $_SERVER['REQUEST_URI']); ?>
<table class="listagem" width="95%" align="center" id="inserirarquivos">
<tr>
<td class="SubTituloCentro">Anexar arquivos</td>
</tr>
<tr>
<td><input type="file" name="Filedata[]" <?php echo $html; ?>></td>
</tr>
<tr>
<td><input type="file" name="Filedata[]" <?php echo $html; ?>></td>
</tr>
<tr>
<td>
<?php echo $adicionar_novos; ?>
</td>
</tr>
<tr>
<td><input type="submit" name="sub" value="Enviar" <?php echo $html; ?>></td>
</tr>
</table>
</form>
<form method="post" name="descricao" action="gravar_obs.php?funcao=<? echo $_REQUEST['funcao']; ?>" target="iframeUpload">
<table class="listagem" width="95%" align="center" id="listaimagens">
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" onclick="return verificaPreenchimento()" id="submit1" <?php echo $html; ?>>
</td>
</tr>
<?php
if($_SESSION['obras2']['supid']) {
    
    if(empty($_SESSION['obras2']['obrid'])){
        $objSup = new Supervisao($_SESSION['obras2']['supid']);
        $_SESSION['obras2']['obrid'] = $objSup->obrid;
    }
	
	$sql = "SELECT arq.arqid, arq.arqdescricao, fot.icoid
			FROM obras2.fotos fot
			LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid 
			WHERE supid='".$_SESSION['obras2']['supid']."' AND obrid='".$_SESSION['obras2']['obrid']."' 
			ORDER BY fotordem";
	
	$arqssup = $db->carregar($sql);
	if($arqssup[0]) {
		$habilitarsubmitsobs = true;
		$controletextarea=0;

        /*
         *
         *  ALTER TABLE obras2.fotos ADD COLUMN icoid INT DEFAULT NULL;
            ALTER TABLE obras2.fotos ADD CONSTRAINT fk_cfotos_reference_itenscomposicaoobra FOREIGN KEY (icoid)
				      REFERENCES obras2.itenscomposicaoobra (icoid) MATCH SIMPLE
				      ON UPDATE CASCADE ON DELETE RESTRICT;
         */

        $itens = pegaItensCronograma();

		foreach($arqssup as $arq) {
			echo "<tr>";
			//echo "<td><img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\"></td>";
			echo "<td></td>";
			echo "<td>";
			echo "<img src='../../slideshow/slideshow/verimagem.php?arqid=".$arq['arqid']."' width=100 height=100>";
			echo "</td>";
			echo "<td>";
            $op = montaSelectItens($arq['arqid'], $arq['icoid']);
            echo $op;
			echo "
			        <textarea {$html}  id='supobs' name='supobs[".$arq['arqid']."]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255 );'  onkeyup='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255);'>".$arq['arqdescricao']."</textarea><br /><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs".$controletextarea."' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>
			        ";
			echo "</td>";
			
		if ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA)) ) {
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex,'".$arq['arqid']."');\">";
		}else{
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir_01.gif\">";
		}
			
			echo "<td>
					{$botao}
				 </td>";
			echo "</tr>";
			$controletextarea++;
		}
	}
}
if($_SESSION['obras2']['emiid']) {
    
    if(empty($_SESSION['obras2']['obrid'])){
        $objEvo = new EvolucaoMi($_SESSION['obras2']['emiid']);
        $_SESSION['obras2']['obrid'] = $objEvo->obrid;
    }
	
	$sql = "SELECT arq.arqid, arq.arqdescricao 
			FROM obras2.fotos fot
			LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid 
			WHERE emiid='".$_SESSION['obras2']['emiid']."' AND obrid='".$_SESSION['obras2']['obrid']."' 
			ORDER BY fotordem";
	
	$arqssup = $db->carregar($sql);

    $itens = pegaItensCronograma();

	if($arqssup[0]) {
		$habilitarsubmitsobs = true;
		$controletextarea=0;
		foreach($arqssup as $arq) {
			echo "<tr>";
			//echo "<td><img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\"></td>";
			echo "<td></td>";
			echo "<td>";
			echo "<img src='../../slideshow/slideshow/verimagem.php?arqid=".$arq['arqid']."' width=100 height=100>";
			echo "</td>"; 
			echo "<td>";
            $op = montaSelectItens($arq['arqid'], $arq['icoid']);
            echo $op;
			echo "<textarea {$html}  id='supobs' name='supobs[".$arq['arqid']."]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255 );'  onkeyup='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255);'>".$arq['arqdescricao']."</textarea><br /><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs".$controletextarea."' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
			echo "</td>";
			
		if ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA)) ) {
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex,'".$arq['arqid']."');\">";
		}else{
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir_01.gif\">";
		}
			
			echo "<td>
					{$botao}
				 </td>";
			echo "</tr>";
			$controletextarea++;
		}
	}
}
?>
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" onclick="return verificaPreenchimento()" id="submit2" <?php echo $html; ?>>
</td>
</tr>
</table>
<?php if($habilitarsubmitsobs) {?>
	<script type="text/javascript">
	document.getElementById('submit1').disabled = false;
	document.getElementById('submit2').disabled = false;
	</script>
<?php } ?>
</form>
</body>
</html>
<script type="text/javascript">
//var opener_container = window.opener.document.getElementById("thumbnails");

var tabela = window.document.getElementById("listaimagens");

var controletextarea=0;

for(var k=0;k<tabela.childNodes.length;k++){
	
	var child = tabela.childNodes;
	
	if(child[k].firstChild){
	
		var imagem = child[k].firstChild;
		
		if(!isNumber(imagem.id)) {
		
			document.getElementById('submit1').disabled = false;
			document.getElementById('submit2').disabled = false;
			var linha  = tabela.insertRow(tabela.rows.length-1);
			linha.id=imagem.id;
			var img  = ""+imagem.id+"";
			var celulaordenacao = linha.insertCell(0);
			celulaordenacao.vAlign="middle";
			celulaordenacao.innerHTML = "<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\" align=\"absmiddle\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\">";
			var celulaimg = linha.insertCell(1);
			celulaimg.innerHTML = "<img src='./resize.php?img=../../../arquivos/obras2/imgs_tmp/"+img+"&w=100&h=100'>";
			var celuladsc = linha.insertCell(2);


            <?
                $op = montaSelectItens(img, $arq['icoid']);
            ?>


			celuladsc.innerHTML = "<?=$op?><textarea  id='supobs' name='supobs["+img+"]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255 );'  onkeyup='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255);'>"+imagem.title+"</textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs"+controletextarea+"' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
			var celuladeleta = linha.insertCell(3);
			celuladeleta.innerHTML = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex);\">";
			controletextarea++;
			
		}
	}

}

</script>


<?

function montaSelectItens($arqid, $default)
{
    $itens = pegaItensCronograma();
    $op =  "<select name='icoid[".$arqid."]'>";

    if(!empty($itens)) {
        foreach ($itens as $item) {
            $selected = ($item['icoid'] == $default) ? 'selected="selected"' : '';
            $op .= "<option $selected value='{$item['icoid']}'>{$item['itcdesc']}</option>";
        }
    }
    $op .=  "</select><br />";

    return $op;
}

function pegaItensCronograma()
{
    global $db;
    $sql = "SELECT
                    i.icoid,
                    io.itcdesc
                FROM obras2.cronograma c
                JOIN obras2.itenscomposicaoobra i ON i.croid = c.croid AND i.obrid = c.obrid
                JOIN obras2.itenscomposicao io ON io.itcid = i.itcid
                WHERE c.crostatus = 'A' AND c.obrid = {$_SESSION['obras2']['obrid']}
                ORDER BY i.icoordem";

    if(isset($_SESSION['obras2']['obrid']) AND !empty($_SESSION['obras2']['obrid']))
        $itens = $db->carregar($sql);
    else
        $itens = array();

    $itens = (empty($itens)) ? array() : $itens;
    return array_merge( array(array('icoid'=>'0', 'itcdesc'=>' -- Selecione a Etapa -- ')), $itens);
}
?>