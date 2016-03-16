<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . "tarefa/classes/Anexo.class.inc";
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";

if($_GET['tarid']){
	# Verifica se existe tarefa
	boExisteTarefa( $_GET['tarid'], true );
	$_SESSION['dados_tarefa']['tarid'] = $_GET['tarid'];	
}

# Verifica Sessão Ativa
verificaSessao(true);

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'tarefa.php?modulo=principal/cadDocumento&acao=A';</script>";
    exit;
} 
if($_FILES["arquivo"] && !$_POST['arqid']){
	$campos	= array("tarid" 	   =>    $_POST['tarid_anexo'],
					"anxdesc"		   => "'{$_POST['anxdesc']}'",
					"tpdid" 		   => "'{$_POST['tpdid']}'",
					"anxassunto" 	   => "'{$_POST['anxassunto']}'",
					"anxnumsidoc" 	   => "'{$_POST['anxnumsidoc']}'",
					"anxnumdoc" 	   => "'{$_POST['anxnumdoc']}'",
					"anxprocessosidoc" => "'{$_POST['anxprocessosidoc']}'"
					);

	$file = new FilesSimec("anexo", $campos ,"tarefa");
	$arquivoSalvo = $file->setUpload();
					
	$obTarefa = new Tarefa($campos['tarid']);
	if($obTarefa->usucpfresponsavel){
		$usucpfresponsavel = $obTarefa->usucpfresponsavel;
		$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);	
		$email = $obTarefa->recuperaEmailPorCpf($usucpfresponsavel);
		$nrtarefa = $obTarefa->pegaTartarefaPorTarid($obTarefa->tarid);
		enviarEmailTarefa($usucpfresponsavel,$email, $arAcompanhamento, $obTarefa->tartitulo, $nrtarefa );
	}
		
	if($arquivoSalvo){
		echo '<script type="text/javascript"> 
    			alert("Operação realizada com sucesso!");
    			window.location.href="tarefa.php?modulo=principal/cadDocumento&acao=A";
    	      </script>';
		die;
	}
} elseif($_POST['arqid'] && $_POST['anxid']) {
	$obAnexo = new Anexo();
	$arCampos = array('anxid','tarid','anxdesc','tpdid','anxassunto','anxnumsidoc','anxnumdoc','anxprocessosidoc');
	$obAnexo->popularObjeto($arCampos);
	$obAnexo->salvar();
    $obAnexo->commit();
    
       
    $_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadDocumento",'');
	unset($obAnexo);
	die;
}
 
if($_GET['arqidDel']){
    $sql = "DELETE FROM tarefa.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="tarefa.php?modulo=principal/cadDocumento&acao=A";
    	  </script>';
    die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

//require_once 'cabecalhoIncludesListaTarefa.inc';

/*
 * Correçao por Alexandre Dourado
 */
if(!$_SESSION['dados_tarefa']['tarid']) {
	die("<script>
			alert('Tarefa não encontrada. Refaça o procedimento.');
			window.location='tarefa.php?modulo=principal/listaTarefas&acao=A';
		  <script>");
}

$tarid = $_SESSION['dados_tarefa']['tarid'];
$obTarefa  = new Tarefa();
$tarTarefa = $obTarefa->pegaTartarefaPorTarid($tarid);

$obAnexo = new Anexo($_GET['anxid']);

// CABECALHO
echo cabecalhoTarefa($tarid);
?>
<br />
<center>
	<div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>
<script src="../includes/prototype.js"></script>
<script src="./js/tarefa.js" type="text/javascript"></script>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="anxid" id="anxid" value="<?php echo $obAnexo->anxid; ?>" />
<input type="hidden" name="arqid" id="arqid" value="<?php echo $obAnexo->arqid; ?>" />
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
        <td width='50%'>
            <input type="file" name="arquivo" id="arquivo"/>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>      
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tarefa / Atividade:</td>
        <td>
        	<?php
        	  $tarid_anexo = $obAnexo->tarid;
	          $sql = "select tarid as codigo,
	            	         tartitulo as descricao
	            		from tarefa.tarefa where _tartarefa = $tarid order by tarid, _tarordem";
	       	  $db->monta_combo('tarid_anexo', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tarid_anexo'); 
        	?>
        </td>
    </tr>
    <tr>
		<td class="SubTituloDireita" >
				Assunto:
		</td>
		<td><? $anxassunto = $obAnexo->anxassunto; ?>
			<?= campo_texto( 'anxassunto', 'S', $permissao_formulario, 'Assunto', 60, 255, '', '','','','','id="anxassunto"'); ?>
		</td>
	</tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo de Documento:</td>
        <td>
        	<?php
        	 $tpdid = $obAnexo->tpdid;
	         $sql = "select tpdid as codigo, tpddescricao as descricao
	                	from tarefa.tipodocumento"; 
	       	 $db->monta_combo('tpdid', $sql, 'S', "--", '', '', '', '100', 'S', 'tpdid'); 
        	?>
        </td>
    </tr>
    <tr>
		<td class="SubTituloDireita" >
				Número do Documento:
		</td>
		<td><?$anxnumdoc = $obAnexo->anxnumdoc; ?>
			<?=campo_texto( 'anxnumdoc', 'N', '', 'Número do Documento', 60, 255, '', '','','','','id="anxnumdoc"'); ?>
			<!--  img border="0" title="Valida número do Documento." width="15px" height="15px" src="../imagens/valida1.gif" align="bottom"/ -->
		</td>
	</tr>
    <tr>
		<td class="SubTituloDireita" >
				Código do SIDOC:
		</td>
		<td>
			<input id="anxnumsidoc" title="Número do documento Sidoc" class="normal" type="text" name="anxnumsidoc" size="60" maxlength="255" value="<?php echo $obAnexo->anxnumsidoc; ?>" onkeyup="this.value=mascaraglobal('#################/####-##',this.value);" onblur="verificaSidoc(this.value)"/>
		</td>
	</tr>
    <tr>
		<td class="SubTituloDireita" >
				Número do Processo:
		</td>
		<td><?$anxprocessosidoc = $obAnexo->anxprocessosidoc;?>
			<?= campo_texto( 'anxprocessosidoc', 'N', '', 'Número do processo Sidoc', 60, 255, '[#].######/####-##', '','','','','id="anxprocessosidoc"'); ?>
		</td>
	</tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td>
        	<?php
        	$anxdesc = $obAnexo->anxdesc;
        	echo campo_textarea( 'anxdesc', 'N', 'S', 'Descrição ', 65 , 4, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição', $anxdesc ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
        <td height="30"><input type="button" name="botao" value="Salvar" onclick="validaForm();"></td>
    </tr> 
</table>
<div id="div_listaDocumento"></div>
<br />
<?
echo "<script type=\"text/javascript\">
		montaListaAnexo('$tarTarefa');
	  </script>"; 
?>
<script language="javascript" type="text/javascript">
if($('anxid').value != ''){
	$('arquivo').disabled = true;
}

function validaForm(){
	var alerta = "";
	if($('arquivo').value == '' && $('arqid').value == ''){
		alerta += "Você deve escolher um arquivo.\n";
	}
	
	if($('tarid_anexo').value == ''){
		alerta += "O campo Tarefa / Atividade é Obrigatório.\n";
	}
	
	if($('anxassunto').value == ''){
		alerta += "O campo Assunto é Obrigatório.\n";
	}
	
	if($('tpdid').value == ''){
		alerta += "O campo Tipo de Documento é Obrigatório.";
	}
	
	if(alerta){
		alert(alerta);
		return false;
	} 
	$('formulario').submit();
}
	 
function excluirAnexo( arqid ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}

function abrirAnexo( arqid ){
	if(arqid){
		window.location='?modulo=principal/cadDocumento&acao=A&download=S&arqid='+arqid;
	}
}

function verificaSidoc( codSidoc ){
	var codSidoc = trim(codSidoc);
 	codSidoc = codSidoc.replace("-","");
 	codSidoc = codSidoc.replace("/",""); 
	if(codSidoc){ 
		var data = 'tipo=pesquisaSidoc&codSidoc='+codSidoc;
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,
			onLoading: $('aguarde_').show(),
			asynchronous: false,
			onComplete: function(r)
			{
				$('divTeste').update(r.responseText);
				//alert( r.responseText );
				var obSidoc = r.responseText.evalJSON();
				
				if(obSidoc.NumeroAnexador){ 
					$('anxprocessosidoc').value = mascaraglobal('[#].######/####-##', obSidoc.NumeroAnexador );  
				} else {
					$('anxprocessosidoc').value = '';
				}
			}
		});
		$('aguarde_').hide()
	}
}
</script>
<div id='divTeste'></div>