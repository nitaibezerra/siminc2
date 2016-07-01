<?php
/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$anexos = new Ted_Form_Anexo();
$arquivo = new Ted_Model_Arquivo();
$fm = new Simec_Helper_FlashMessage('ted/ted');

if (isset($_GET['download']) && $_REQUEST['download'] == 's' && is_numeric($_REQUEST['arqid'])) {
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	unset($_REQUEST['download']);
	$arqid = (int) $_REQUEST['arqid'];
	ob_get_clean();
    $arquivo->getDownload($arqid);
	echo "<script> window.close(); </script>";
	die;
}

if (isset($_GET['removerAnexo'])) {
	echo $arquivo->desativarAnexo($_GET['removerAnexo']);
	die();
}

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

//Setando o TCPID da sessão no campo do formulário
$anexos->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

if (isset($_POST['save-anexo'])) {
	$dados = array_merge($_POST,$_FILES);
	$retorno = $arquivo->inserirAnexo($dados, Ted_Model_Arquivo::ABA_ANEXO);

    $msg = ($retorno) ? 'Anexo inserindo com sucesso!' : 'Falha ao tentar incluir o anexo.';
    $erro = ($retorno) ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

    $_SESSION['flashmessage'] = array(
        'message' => $msg,
        'type' => $erro
    );
}

//Anexa uma mensagem ao flashmessage, se houver registro de mensagem na sessao
if (is_array($_SESSION['flashmessage']) && !empty($_SESSION['flashmessage'])) {
    if (array_key_exists('message', $_SESSION['flashmessage'])) {
        $fm->addMensagem($_SESSION['flashmessage']['message'], $_SESSION['flashmessage']['type']);
        unset($_SESSION['flashmessage']);
    }
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript">
var quantidadeCampos = 0
  , ctrlAnexo = window.ctrlAnexo = {
        gestor: <?=(int)Ted_Utils_Model::possuiPerfilGestor();?>
    };

function downloadAnexo(id) {
	return window.open('ted.php?modulo=principal/termoexecucaodescentralizada/anexos&acao=A&ted=<?=$_GET['ted'];?>&download=s&arqid='+id,'blank','height=350,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' ).focus();
}
function fechar(id) {
	$('#campo-anexo'+id).remove().fadeOut();
	quantidadeCampos--;
	if (quantidadeCampos == 0) {
		$('#btn-salvar-anexo').fadeOut();
		$('#anexo-form').fadeOut();
	}
}

function desativarAnexo(id) {

    if (!ctrlAnexo.gestor) {
        bootbox.alert("O arquivo não pode ser apagado, favor entrar em contato com a CGSO/MEC");
        return false;
    }

    bootbox.confirm("Deseja realmente apagar o arquivo?", function(result){
        if (!result) return false;

        var url ="ted.php?modulo=principal/termoexecucaodescentralizada/anexos&acao=A&ted=<?=$_GET['ted']; ?>&removerAnexo="+id;

        $.post(url,function(data) {
            if (data == 1) {
                bootbox.alert('Anexo excluído com sucesso!');
                document.location.href= location.href;
            }
        });
    });
}

$(document).ready(
		function(){
			$('#anexo-form').hide();			
			
			$('#br').change(function(){
				//console.log('Valor: ' + $(this).val());
			});

			$('#br').change();
			
			$('#btn-salvar-anexo').hide();
			$('#novo-anexo').click(
				function(){			
					//console.log(quantidadeCampos);
					if(quantidadeCampos == 0){	
						$('#anexo-form').fadeIn();	
					}
					$('#anexo-form').append(
					'<section id="campo-anexo'+quantidadeCampos+'">'	
					+'<section class="form-group">'
					+'	<div class="col-md-offset-11">'
					+'		<button type="button" id="fechar'+quantidadeCampos+'" onclick="fechar('+quantidadeCampos+');" class="btn btn-warning btn-xs"><span class=" glyphicon glyphicon-warning-sign"></span> Cancelar</button>'
					+'	</div>'
					+'</section>'			
					+'<section class="form-group">'									        			       			
			    	+'	<label class="control-label col-md-2" for="anexo'+quantidadeCampos+'">Anexo:</label>'
			    	+'	<div class="col-md-10">' 	    	
			    	+'		<input type="hidden" name="anexoCod[]" value="'+quantidadeCampos+'"/>'						
			    	+'		<input type="file" required name="anexo_'+quantidadeCampos+'" id="anexo'+quantidadeCampos+'" />'
			    	+'	</div>'			            												
					+'</section>'
					+'<section class="form-group">'			
					+'	<label class="control-label col-md-2" for="descricao'+quantidadeCampos+'">Descrição:</label>'
					+'	<div class="col-md-10">'
					+'		<textarea class="form-control" cols="2" name="descricaoanexo[]" id="descricao'+quantidadeCampos+'" maxlength="255"></textarea>'
					+'	</div>'																	
					+'</section>'
					+'</section>');
					quantidadeCampos++;		
					$('#btn-salvar-anexo').fadeIn();
				}
			);
		    //console.log($('#tcpusucpfparecer'));
		}
	);
</script>
<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Anexos</li>
	</ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

	<section class="col-md-10">
		<section class="well">
			<h4 class="text-center">Anexos</h4>
		</section>
		<section class="">
            <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
			<?php $arquivo->listarAnexos(Ted_Model_Arquivo::ABA_ANEXO); ?>			
		</section>
        <?php echo $anexos->showForm(); ?>
	</section>
</section>

<?php
//echo md5_decrypt_senha('826FScuxF9oU9cmzis2yAr1z/6m9MKEE6YAJmOz4Qn4=', ''); exit; //37897412806 - insp20

/*$srtSQL = "
	select
		h.hstid, min(h.htddata) AS execucao, h.docid,
		(select tcpid from ted.termocompromisso where docid = h.docid) as tcpid,
		(select max(crdmesexecucao) from ted.previsaoorcamentaria where tcpid = (select tcpid from ted.termocompromisso where docid = h.docid)) AS periodo
	from workflow.historicodocumento h
	where h.aedid in (1609, 1618, 2440)
	group by h.hstid
";

$historico = $db->carregar($srtSQL);

foreach ($historico as $hst) {

    if ($hst['periodo'] > 0) {
        $data = new DateTime($hst['execucao']);
        $data->modify("+{$hst['periodo']} month");
        $vigencia = $data->format('Y-m-d');

        $sqlTemp = "UPDATE ted.termocompromisso SET dtvigenciaincial = '%s' , dtvigenciafinal = '%s' WHERE tcpid = %d";
        $dml = sprintf($sqlTemp, $hst['execucao'], $vigencia, $hst['tcpid']);
        echo $dml.';<br>';
    }
}*/
?>