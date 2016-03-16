<?php


switch ($_REQUEST["requisicao"]) {

	case "salvar":
            $arrCamposObrigatorio = array('mdonome' => 'Nome do Modelo \n', 'tpdcod' => 'Tipo do Documento \n', 'mdoconteudo' => 'Descrição \n');
            $intValidador = 0;
            $strMensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s):\n\n';
            foreach($arrCamposObrigatorio as $key => $valor){
                if (empty($_POST[$key])) {
                    $intValidador++;
                    $strMensagem .= $valor;
                }
            }

            if ($intValidador > 0){
                echo "<script>alert('$strMensagem');window.location=window.location</script>";
            }

            if( strpos($_POST["mdoconteudo"], '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
                $_POST["mdoconteudo"] = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $_POST["mdoconteudo"] );
            } else {	
                $_POST["mdoconteudo"] = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $_POST["mdoconteudo"] );
            }

            $gtdValidacao = 1;
            if( !empty($_POST["iutid"]) ){
                $gtdValidacao = 2;
            }

            $mdovalidacaofnde = ($_POST['mdovalidacaofnde'] ? $_POST['mdovalidacaofnde'] : 'f');
            $iutid = ($_POST["iutid"] ? $_POST["iutid"] : 'null');

            $mdoConteudo = substituir_char_especiais_word($_POST["mdoconteudo"]);
            if( $_POST["mdoid"] ){			
                $sql = "UPDATE par.modelosdocumentos
                        SET
                            tpdcod = {$_POST["tpdcod"]},
                            mdoqtdvalidacao = $gtdValidacao,
                            iutid = {$iutid}, 
                            mdonome = '{$_POST["mdonome"]}', 
                            mdoconteudo = '{$mdoConteudo}',
                            mdovalidacaogestor = '{$_POST['mdovalidacaogestor']}',
                            mdovalidacaofnde = '{$mdovalidacaofnde}',
                            mdotipovinculado = ".($_POST['mdotipovinculado'] ? "'".$_POST['mdotipovinculado']."'" : 'null').",
                            mdodocumentoex = ".($_POST['mdodocumentoex'] ? "'".$_POST['mdodocumentoex']."'" : 'null')."
                        WHERE
                            mdoid = {$_POST["mdoid"]} ";
            }else{
                $sql = "INSERT INTO par.modelosdocumentos( tpdcod,
                            iutid, 
                            mdonome, 
                            mdoconteudo, 
                            mdoqtdvalidacao, 
                            mdostatus, 
                            usucpf, 
                            mdodatainclusao,
                            mdovalidacaogestor,
                            mdovalidacaofnde,
                            mdotipovinculado,
                            mdodocumentoex )
                        VALUES( {$_POST["tpdcod"]},
                            {$iutid},
                            '{$_POST["mdonome"]}',
                            '{$mdoConteudo}',
                            $gtdValidacao,
                            'A',
                            '{$_SESSION["usucpf"]}',
                            'now()',
                            '{$_POST['mdovalidacaogestor']}',
                            '{$mdovalidacaofnde}',
                            ".($_POST['mdotipovinculado'] ? "'".$_POST['mdotipovinculado']."'" : 'null')." ,
                            ".($_POST['mdodocumentoex'] ? "'".$_POST['mdodocumentoex']."'" : 'null')." 
                        )";
            }
            $db->executar( $sql );
            $db->commit();		
            $db->sucesso( "principal/modeloDocumentos", "" );
		
	break;	
	case "alterar":
		$sql = "SELECT mdoid, mdonome, tpdcod, iutid, mdoconteudo, mdovalidacaogestor, mdovalidacaofnde, mdotipovinculado, mdodocumentoex FROM par.modelosdocumentos WHERE mdoid = {$_POST["mdoid"]}";		
		$dados = $db->pegaLinha( $sql );		
                
		$mdoConteudo = substituir_char_especiais_word($dados["mdoconteudo"]);
		$dados["mdonome"] 	  = iconv("ISO-8859-1", "UTF-8", $dados["mdonome"]);
		$dados["mdoconteudo"] = iconv("ISO-8859-1", "UTF-8", $mdoConteudo);		
		echo simec_json_encode($dados);		
		die;
		
	break;
	
	case "excluir":
		$sql = "UPDATE par.modelosdocumentos SET mdostatus = 'I' WHERE mdoid = {$_REQUEST["mdoid"]}";	
		$db->executar( $sql );
		$db->commit();		
		$db->sucesso( "principal/modeloDocumentos", "" );
	break;
	
}


require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Modelo de Documentos', '');

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
	
</script>
<form id="formulario" action="" method="post">
	<input type="hidden" name="requisicao" value="salvar"/>
	<input type="hidden" name="mdoid" id="mdoid" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
		<tr>
			<td class="subtitulodireita">Nome do Modelo:</td>
			<td>
				<?php echo campo_texto( 'mdonome', 'S', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="mdonome"'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo do Documento:</td>
			<td>
				<?php 				
					$sql = "SELECT
								tpdcod as codigo,
								tpddsc as descricao
							FROM
								public.tipodocumento
							WHERE
								tpdstatus = 'A'
							ORDER BY
								descricao";
					
					$db->monta_combo("tpdcod", $sql, 'S', "Selecione...", 'pegaTipoDoc', '', '', '', 'S', 'tpdcod');
				?>
			</td>
		</tr>
		<tr id="tr_doc_vinc" style="display: none;">
			<td class="subtitulodireita">Documento Vinculado:</td>
			<td>
				<?php 				
					$sql = "select 
								mdoid as codigo,
							    mdonome || ' (' || tpd.tpddsc || ')' as descricao
							from 
								par.modelosdocumentos mdo
							inner join public.tipodocumento tpd ON tpd.tpdcod = mdo.tpdcod
							where
								mdovisivel = 'S'
							    and mdostatus = 'A'";
					
					$db->monta_combo("mdotipovinculado", $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'mdotipovinculado');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Validação Gestor:</td>
			<td>
				<input type="radio" value="t" id="mdovalidacaogestor" name="mdovalidacaogestor" <? if($_REQUEST["mdovalidacaogestor"] == "t") { echo "checked"; } ?> /> Sim
				<input type="radio" value="f" id="mdovalidacaogestor" name="mdovalidacaogestor" <? if($_REQUEST["mdovalidacaogestor"] == "f" || empty($_REQUEST["mdovalidacaogestor"]) ) { echo "checked"; } ?> /> Não
			</td>
		</tr>		
		<tr>
			<td class="subtitulodireita">Validação FNDE:</td>
			<td>
				<input type="radio" value="t" id="mdovalidacaofnde" name="mdovalidacaofnde" <? if($_REQUEST["mdovalidacaofnde"] == "t") { echo "checked"; } ?> /> Sim
				<input type="radio" value="f" id="mdovalidacaofnde" name="mdovalidacaofnde" <? if($_REQUEST["mdovalidacaofnde"] == "f" || empty($_REQUEST["mdovalidacaogestor"]) ) { echo "checked"; } ?> /> Não
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ex Ofício:</td>
			<td>
				<input type="radio" value="t" id="mdodocumentoex" name="mdodocumentoex" <? if($_REQUEST["mdodocumentoex"] == "t") { echo "checked"; } ?> /> Sim
				<input type="radio" value="f" id="mdodocumentoex" name="mdodocumentoex" <? if($_REQUEST["mdodocumentoex"] == "f" || empty($_REQUEST["mdodocumentoex"]) ) { echo "checked"; } ?> /> Não
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Necessário assinatura de outra entidade?:</td>
			<td>
				<?php 				
					$sql = "select 
								iutid as codigo,
								iuttipo as descricao
							from par.instrumentounidadeentidadetipo
							order by iuttipo";
					
					$db->monta_combo("iutid", $sql, 'S', "Selecione...", '', '', '', '', 'S', 'iutid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Descrição:</td>
			<td>
				<textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="80" style="width:85%"></textarea>
				<input type="hidden" name="textohidden" id="textohidden" value="">
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Dados a serem adicionados:</td>
			<td>
				<?php
					$sql = "SELECT
								teccampo as codigo,
								tecdescricao as descricao
							FROM
								par.termocampos
							WHERE
								tecstatus = 'A'
							ORDER BY
								descricao";
													
					$db->monta_combo("teccampo", $sql, 'S', "Selecione...", '', '', '', '600', 'N', '');
					
				?>
				<input type="button" id="adicionar" value="Adicionar" onclick="adicionaMacro();"/>
			</td>
		</tr>
		<tr bgcolor="#c0c0c0">
			<td></td>
			<td>
				<input type="button" id="bt_salvar" value="Salvar" onclick="salvarModelo();" style="cursor: pointer;"/>
				<input type="button" id="bt_voltar" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="subtitulocentro">Modelos de Documento Cadastrados</td>
	</tr>
</table>

<?php

	$sql = "SELECT
				'<center>
					<img src=\"/imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"consultarDocumento( ' || mdoid || ' ); \" title=\"Visualizar Impressão\">
					<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDocumento( ' || mdoid || ' );\" title=\"Editar\">
					<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirDocumento( ' || mdoid || ' );\" title=\"Excluir\">
				 </center>' as acao,
				mdoid as codigo, 
				tpddsc as tipo,
				mdonome,
				to_char(mdodatainclusao,'DD/MM/YYYY') as data,
				usunome
			FROM
				par.modelosdocumentos em
			INNER JOIN
				public.tipodocumento tp ON tp.tpdcod = em.tpdcod
			INNER JOIN
				seguranca.usuario su ON su.usucpf = em.usucpf
			WHERE
				mdostatus = 'A'
				and em.mdovisivel = 'S'
			ORDER BY
				mdonome, mdoid";

	$cabecalho = array( "Ações", "Código", "Tipo de Documento", "Nome do Modelo", "Data de Inclusão", "Inserido Por" );
	$db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );

?>

<script type="text/javascript">
	
	var tecCampo = document.getElementsByName("teccampo")[0];
	
	function pegaTipoDoc(valor){
		
		if( valor == 21 ){
			$('tr_doc_vinc').style.display 	= '';
		} else {
			$('mdotipovinculado').value = '';
			$('tr_doc_vinc').style.display 	= 'none';
		}
	}
	
	function salvarModelo() {
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;
		
		var mdonome = document.getElementById('mdonome').value;
		if (mdonome == ''){
			mensagem += 'Nome do Modelo \n';
			validacao = false;
		}
		
		var tpdcod = document.getElementById('tpdcod').value;
		if (tpdcod == ''){
			mensagem += 'Tipo do Documento \n';
			validacao = false;
		}
		
		
		var mdoconteudo = tinyMCE.get('mdoconteudo').getContent( {format : 'text'} );
		if (mdoconteudo == ''){
			mensagem += 'Descrição \n';
			validacao = false;
		}
		
		if( !validacao ){
			alert(mensagem);
			return false;
		}else{
			document.getElementById('formulario').submit();
		}
		
	}
	
	function adicionaMacro() {
		var macro = tecCampo.options[tecCampo.selectedIndex].value;
		tinyMCE.execCommand('mceInsertContent', false, macro);
	}
	
	function consultarDocumento( mdoid ){
		window.open( 'par.php?modulo=principal/consultaDocumento&acao=A&mdoid=' + mdoid, 'consulta', 'width=800,height=600,scrollbars=yes,menubar=yes' );
	}
	
	function alterarDocumento ( mdoid ){
	
		var url = 'par.php?modulo=principal/modeloDocumentos&acao=A';
		var parametros = '&requisicao=alterar&mdoid=' + mdoid;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					var json = resp.responseText.evalJSON();
					
					$('mdoid').value 	   = json.mdoid;
					$('mdonome').value     = json.mdonome;
					$('tpdcod').value 	   = json.tpdcod;
					$('iutid').value 	   = json.iutid;
					
					if( json.tpdcod == 21 ){
						$('tr_doc_vinc').style.display 	= '';
						$('mdotipovinculado').value 	= json.mdotipovinculado;
					} else {
						$('tr_doc_vinc').style.display 	= 'none';
						$('mdotipovinculado').value 	= '';
					}
					
					if(json.mdovalidacaogestor == 't'){
						$('mdovalidacaogestor').checked = true;
					} else {
						$('mdovalidacaogestor').checked = false;
					}
					
					if(json.mdovalidacaofnde == 't'){
						$('mdovalidacaofnde').checked = true;
					} else {
						$('mdovalidacaofnde').checked = false;
					}
					
					if(json.mdodocumentoex == 't'){
						$('mdodocumentoex').checked = true;
					} else {
						$('mdodocumentoex').checked = false;
					}
					tinyMCE.get('mdoconteudo').setContent( json.mdoconteudo );
				}
			}
		);
	
	}
	
	function excluirDocumento( mdoid ){
	
		if ( confirm( "Deseja realmente excluir este Modelo de Documento?" ) ){
			window.location.href = "par.php?modulo=principal/modeloDocumentos&acao=A&requisicao=excluir&mdoid=" + mdoid;
		}
	
	}

</script>