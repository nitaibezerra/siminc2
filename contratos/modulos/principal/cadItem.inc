<?php 
if ( $_POST['acaoForm'] ){
	if(!$_POST['coiid']){
		 $sql = "INSERT INTO evento.coitem
                            (
                            coidsc,
                            coiespecificacao,
                            umeid,
                            coicodsiasg,
                            coivlrreferenciamin,
                            coivlrreferenciamax,
                            coicotacao
                            )
                        VALUES
                            (
                            '".$_POST['coidsc']."',
                            '".$_POST['coiespecificacao']."',
                            ".$_POST['umeid'].",
                            '".$_POST['coicodsiasg']."',
                            '".str_replace(",",".", str_replace(".","",$_POST['coivlrreferenciamin'] ) )."',
                            '".str_replace(",",".", str_replace(".","",$_POST['coivlrreferenciamax'] ) )."',
                            '".$_POST['coicotacao']."' 
                            ) returning coiid";

		$coiid = $db->pegaUm( $sql );
	} else {
		$coiid = $_POST['coiid'];
		$sql = "UPDATE evento.coitem
                        SET
                            coidsc = '".$_POST['coidsc']."',
                            umeid = ".$_POST['umeid'].",
                            coiespecificacao = '".$_POST['coiespecificacao']."',
                            coicodsiasg = '".$_POST['coicodsiasg']."',
                            coivlrreferenciamin =  '".str_replace(",",".", str_replace(".","",$_POST['coivlrreferenciamin'] ) )."',
                            coivlrreferenciamax =  '".str_replace(",",".", str_replace(".","",$_POST['coivlrreferenciamax'] ) )."', 
                            coicotacao = '".$_POST['coicotacao']."'
                        WHERE
                            coiid = '{$coiid}'
                        ";
		$db->executar( $sql );	
	}
	$db->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadItem","&coiid=".$coiid);
	die;
}

if($_REQUEST['acao'] == 'A' && $_REQUEST['coiid']){
	$sql = "select
				coiid,
				coidsc,
				coiespecificacao,
				umeid,
				coicodsiasg,
				coalesce(coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
				coalesce(coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax,
				coicotacao
	from evento.coitem where coiid = ".$_REQUEST['coiid'];
	$res =  $db->carregar($sql);
	//foreach($res[0] as $k=>$v) {${$k}=$v;};
	extract($res[0]); 
	
	$coivlrreferenciamin = number_format($coivlrreferenciamin, 2, ',', '.');
	$coivlrreferenciamax = number_format($coivlrreferenciamax, 2, ',', '.');
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?> 
    <script src="../includes/prototype.js"></script>
    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
    </style>
    <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
    <script language="JavaScript">
//Editor de textos
tinyMCE.init({
	theme : "advanced",
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	width : "450px",
	entity_encoding : "raw"
	});
</script>
<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="acaoForm" id=acaoForm value="1" />
	<input type="hidden" name="coiid" id="coiid" value="<? echo $coiid ?>" />
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
      <tr>
      	 <td class="subtitulodireita"> 	
      	 	Código SIASG:
      	 </td>
      	 <td align="left">
      	 	<?= campo_texto('coicodsiasg', 'S', $somenteLeitura, 'Código SIASG', 10, 100, '##########', '', 'left', '',  0, 'id="coicodsiasg" onblur="MouseBlur(this);"', ' ','', "" ); ?>
         </td>
	  </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      	 	Descrição:
      	 </td>
      	 <td align="left">
      	 	<?= campo_texto( 'coidsc', 'S', $somenteLeitura, 'Descrição', 67, 67, '', '','','','','id="coidsc"'); ?>            
      	 </td>
      </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      	 	Especificação:
      	 </td>
      	 <td align="left">
      	 	<textarea name="coiespecificacao" rows="15" cols="80" class="text_editor_simple"><?= $coiespecificacao ?></textarea>
      	 </td>
      </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      		Valor Mínimo:
      	 </td>
      	 <td align="left">
      	 	<?= campo_texto('coivlrreferenciamin', 'S', $somenteLeitura, 'Valor Mínimo', 20, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="coivlrreferenciamin" onblur="MouseBlur(this);"', ' ','', "this.value = mascaraglobal('#.###.###.###,##',  document.getElementById('coivlrreferenciamin').value )" ); ?>
         </td>
      </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      		Valor Máximo:
      	 </td>
      	 <td align="left">
      	 	<?= campo_texto('coivlrreferenciamax', 'S', $somenteLeitura, 'Valor Máximo', 20, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="coivlrreferenciamax" onblur="MouseBlur(this);"', ' ','', "this.value = mascaraglobal('#.###.###.###,##',  document.getElementById('coivlrreferenciamax').value )" ); ?>
         </td>
      </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      	 	Unidade de Medida:
      	 </td>
      	 <td align="left">
      	 	<?php
      	 	$sql = "SELECT 
			umeid as codigo,
			umedescricao as descricao
			FROM evento.unidademedida where umesigla is not null";

        	$db->monta_combo("umeid",$sql,'S',"Selecione a Unidade de Medida",'','', '', 200, 'S', 'umeid');
      		?>
      		</td>
      </tr>
      <tr>
      	 <td class="subtitulodireita"> 	
      	 	Forma de Cotação:
      	 </td>
      	 <td align="left">
      	 	<?= campo_texto( 'coicotacao', 'N', $somenteLeitura, '', 67, 67, '', '','','','','id="coicotacao"'); ?>            
      	 </td>
      </tr>
	  <tr>
		 <td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
         	<input type="button" name="btnGravar" id="btnGravar" value="Salvar" onclick="enviaForm();" /> <? if($_REQUEST['acao'] == 'A'){ ?><input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadItem&acao=I';" /><? } ?>	
         </td>
     </tr>
      </table>
 </form>
<script type="text/javascript">
	function enviaForm(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		var umeid = document.getElementById('umeid');
		if(umeid.value == ""){
			alert('O campo Unidade de Medida é Obrigatório.');
			return false;
		}
		
		campos[0] 			= "coicodsiasg";
		campos[1] 			= "coidsc";
		campos[2]			= "coivlrreferenciamin"; 
		campos[3]			= "coivlrreferenciamax";
		campos[4]			= "umeid";
						 
		tiposDeCampos[0] 	= "texto";
		tiposDeCampos[1] 	= "texto"; 
		tiposDeCampos[2] 	= "texto";
		tiposDeCampos[3] 	= "texto"; 
		tiposDeCampos[4] 	= "texto"; 
		
		validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	}
</script>