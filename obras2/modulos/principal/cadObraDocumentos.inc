<?php 

// empreendimento || obra || orgao
verificaSessao( 'obra' );

switch ( $_REQUEST['requisicao'] ){
       case 'tornarimportante':


        $regAtividade = new ObrasArquivos( $_REQUEST['rgaid'] );

        $regAtividade->oarimp = 't';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;

    case 'retirarimportancia':
        $regAtividade = new ObrasArquivos( $_REQUEST['rgaid'] );

        $regAtividade->oarimp = 'f';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;
}

function excluir(){
    
    
    if(possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE))){
        echo "<script type=\"text/javascript\">
			alert('Você não tem permissões suficientes para excluir esse documento!');
			document.location.href = document.location.href;
		  </script>";
    }else{
	
	global $db;
	
	$obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );
	$obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
				  ->salvar();
	$obrasArquivos->commit();
	
	echo "<script type=\"text/javascript\">
			alert('Operação realizada com sucesso!');
			document.location.href = document.location.href;
		  </script>";
    }
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$obraArquivo = new ObrasArquivos();
	$arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
	$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
	
	$file = new FilesSimec(null,null,$eschema);
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	
	die('<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>');
}

function salvar(){
	
	global $db;
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$campos	= array("obrid"		=> $_SESSION['obras2']['obrid'],
					"oardesc" 	=> "'".$_POST['oardesc']."'",
					"tpaid" 	=> $_POST['tpaid'],
					"oardata" 	=> " now() ");
	
	$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
	if($_FILES["arquivo"]){	
		
		$arquivoSalvo = $file->setUpload($_POST['oardesc']);	
		if($arquivoSalvo){
			echo '<script type="text/javascript"> 
                                    alert("Arquivo anexado com sucesso.");
                                    document.location.href = document.location.href;
                              </script>';
		}
	}
}


if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
if( $_GET['acao'] != 'V' ){
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
		$db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
	}elseif( $_SESSION['obras2']['obrid'] ){
		if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
		}else{
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
		}
	}else{
		$db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
	}
	
	$habilitado = true;
	$habilita 	= 'S';
	
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
	$habilitado = false;
	$habilita 	= 'N';
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita = 'N';
}

$obrasArquivos 	= new ObrasArquivos();
$orgid 			= $_SESSION['obras2']['orgid'];

//$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
//$empreendimento->montaCabecalho();
echo cabecalhoObra($obrid);
echo "<br>";
monta_titulo( 'Documentos da Obra', '</td></tr><tr><td style="background: #f00; color: #fff"><img src="/imagens/atencao.png" /> Insira documentos em formato .pdf, .docx, Documentos não assinados não tem validade.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
 
$(document).ready(function(){
	$('[type="text"]').keyup();
	$('.incluir').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Campo obrigatório.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvar');
		$('#formObraArquivos').submit();
	});
	$('.download').click(function(){
		$('#req').val('download');
		$('#arqid').val( $(this).attr('id') );
		$('#formObraArquivos').submit();
	});
	$('.excluir').click(function(){
		if ( confirm('Deseja apagar o documento?') ){
			$('#req').val('excluir');
			$('#oarid').val( $(this).attr('id') );
			$('#formObraArquivos').submit();
		}
	});

    $('img[src="/imagens/estrela.png"]').closest('tr').attr('style','background:#FFED32');
});

function retirarImportancia( rgaid ){
    location.href = '?modulo=principal/cadObraDocumentos&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=retirarimportancia';
}
function tornarImportante( rgaid ){
    location.href = '?modulo=principal/cadObraDocumentos&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=tornarimportante';
}
</script>
<form method="post" name="formObraArquivos" id="formObraArquivos" enctype="multipart/form-data">
	<input type="hidden" name="req"  	id="req"	value="" />
	<input type="hidden" name="arqid" 	id="arqid"	value="" />
	<input type="hidden" name="oarid" 	id="oarid"	value="" />
	<input type="hidden" name="demid" value="<?=$_SESSION['obras2']['demid']?>" />
	<?php if( $habilitado ): ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class="SubTituloDireita" width="20%">Descrição:</td>
                        <td><?php echo campo_texto('oardesc','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc"', '');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo:</td>
                        <td>
                        <?php
                            $tipoArquivo = new TipoArquivo();
                            $sql = $tipoArquivo->listaCombo();
                            $db->monta_combo("tpaid", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpaid');
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr>
                        <td style="background-color:#DCDCDC" colspan="3" align="center">
                            <?php if( $habilitado ){ ?>
                            <input type="button" name="salvar" class="incluir" value="Anexar Arquivo"/>
                            <?php }?>
                        </td>
                    </tr>
		</table>
	<?php endif; ?>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
//        if($blockEdicao){
//            echo '<script type="text/javascript">';
//            echo " setTimeout(bloqueiaForm('formObraArquivos'), 500);
//                   function bloqueiaForm(idForm){
//                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
//                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
//                   }
//                 ";
//            echo '</script>';
//        }
?>


<?php 
$param = array("obrid" => $obrid, "not(img)" => true);
if ( $habilitado == false || $blockEdicao){
	$param['block_img_excluir'] = true;
}

$sql = $obrasArquivos->listaSql( $param );

if( $_REQUEST['ordemlista'] == 4 ){ // ordenacao pelo nome do arquivo

	$_SESSION['obras2']['order_extra']['campo'] = 'a.arqnome';

	if( $_SESSION['obras2']['order_extra']['campo'] == 'a.arqnome' && $_SESSION['obras2']['order_extra']['ordem'] == 'ASC' )
		$_SESSION['obras2']['order_extra']['ordem'] = 'DESC';
	else
		$_SESSION['obras2']['order_extra']['ordem'] = 'ASC';

	$_REQUEST['ordemlista'] = $_SESSION['obras2']['order_extra']['campo'];
	$_REQUEST['ordemlistadir'] = $_SESSION['obras2']['order_extra']['ordem'];

}else if( $_REQUEST['ordemlista'] == 5 ){ // ordenacao pela data

	$_REQUEST['ordemlista'] = 'oardtinclusao';
	$_SESSION['obras2']['order_extra']['campo'] = 'oardtinclusao';
	if( $_SESSION['obras2']['order_extra']['campo'] == 'a.arqnome' && $_SESSION['obras2']['order_extra']['ordem'] == 'ASC' )
		$_SESSION['obras2']['order_extra']['ordem'] = 'DESC';
	else
		$_SESSION['obras2']['order_extra']['ordem'] = 'ASC';

	$_REQUEST['ordemlista'] = $_SESSION['obras2']['order_extra']['campo'];
	$_REQUEST['ordemlistadir'] = $_SESSION['obras2']['order_extra']['ordem'];

}


$cabecalho 	= array("Ação", "Tipo Arquivo", "Descrição", "Arquivo", "Data de Inclusão", "Gravado por");
$db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");

?>