<?php
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "includes/classes/questionario/TelaBootstrap.class.inc";
	require_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	require_once APPRAIZ . "par3/classes/controle/CampoExternoControle.class.inc";
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$inuid = $_REQUEST['inuid'];

	$controleUnidade = new Par3_Controller_InstrumentoUnidade();
	$modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);

	$qrpid = $modelUnidade->retornarQrpidQuestoesEstrategicas();

	switch ($_REQUEST['req']){
		case 'salvarEscolaGremio':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasGremio::salvarEscolaGremio($_REQUEST);
		    $modelEscola = new Par3_Model_QuestoEsestrategicasEscolasGremio();
		    echo $modelEscola->retornarQtdEscolas($_REQUEST['qrpid'], $_REQUEST['perid']);
		    die();
		    break;
		case 'formEscolasGremio':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasGremio::formEscolasGremio();
		    die();
		    break;
		case 'salvarEscolaCE':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasCe::salvarEscolaCE($_REQUEST);
		    $modelEscola = new Par3_Model_QuestoEsestrategicasEscolasCe();
		    echo $modelEscola->retornarQtdEscolas($_REQUEST['qrpid'], $_REQUEST['perid']);
		    die();
		    break;
		case 'formEscolasCe':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasCe::formEscolasCe();
		    die();
		    break;
		case 'salvarArquivoQuestionario':
		    CampoExternoControle::salvar();
		    simec_redirecionar('par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid='.$_REQUEST['inuid']);
		    break;
		case 'excluirArquivo':
		    $modelQuestoes = new Par3_Model_QuestoesEstrategicasAnexos();
		    $modelQuestoes->excluirPorArqid($_REQUEST['arqid']);
		    simec_redirecionar('par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid='.$_REQUEST['inuid'], 'success', 'Arquivo excluído.');
		    break;
	}
?>
<style>
.ibox-content-round-gray{
    width:99%;
    background-color: #f3f3f4;
    float: center;
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
    border: 0px solid #000000;
}

.iconUnidade{
    margin-top: 0px !important;
    margin-left: 0px !important;
    font-size: 15px !important;
    background-color:black;
    border-radius: 20px 20px 20px 20px;
    -moz-border-radius: 20px 20px 20px 20px;
    -webkit-border-radius: 20px 20px 20px 20px;
    color: yellow !important;
}

.menuUnidade{
    border-bottom: 1px solid #e3e3e3;
    padding: 5px;
    cursor:pointer;
}

.menuUnidade:hover{
    font-weight: bold;
}

.menuSelecionado{
    background-color: #F5F5DC;
    cursor: default !important;
}

@media (max-width: 1450px) {
    .ibox-content-round-gray{
        margin-top: 0.5%
    }
}

@media (max-width: 1200px) {
    .i1450 {
        display: inline;
    }
    .custom-col-10{
        padding: 5px 0px 0px 3.8%;
    }
    .marcador{
        position:absolute;
        border-radius: 100% 0% 0% 100%;
        -moz-border-radius: 100% 0% 0% 100%;
        -webkit-border-radius: 100% 0% 0% 100%;
        border: 0px;
        width:50px;
        height:10px;
        margin-top:21px;
        margin-left:-1%;
    }
}
#telaarvore #bloco {
	margin: 0px;
}
#telaarvore #bloco, #telaarvore .dtree {
	font-size: 13px;
}
#telacentral table {
	width: 100%;
}
</style>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']?>"/>
	            <?php echo $simec->title($controleUnidade->pegarNomeEntidade($_REQUEST['inuid'])); ?>
                <br>
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
				        <div class="ibox float-e-margins">
            				<div class="ibox-content-round-gray ibox-content">
							<?php
           						$habilitado = true;
           						$msgDesabilitado = 'As questões pontuais só estarão disponíveis para edição no periodo de {dt_inicio} a {dt_fim}';
          						$tela = new TelaBootstrap(array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'relatorio' => 'modulo=relatorio/relatorioQuestionario&acao=A', 'habilitado' => 'S', 'msgDesabilitado' => ''));
               				?>
                			</div>
        				</div>
        			</div>
    			</div>
			</div>
		</div>
	</div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true">
	<div class="modal-dialog" style="width: 850px">
		<div id="html_modal-form" style="width: 850px">
		</div>
	</div>
</div>
<script>
function excluirArquivo(arqid){
	var url = '?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A';
    $(window).attr('location', url+'&inuid=<?php echo $inuid; ?>&req=excluirArquivo&arqid='+arqid);
}

function atulizaEscolaCE(entid){
	var qrpid = $('#qrpid').val();
	var perid = $('#perid_escola').val();
	var check = $(this).attr('checked');
	var param = '&req=formEscolasCe&qrpid='+qrpid+'&perid='+perid+'&check='+check;
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: param,
   		async: false,
   		success: function(resp){
			$('#html_modal-form').html(resp);
		    $('#modal-form').modal();
   		}
 	});
}

$(document).ready(function()
{
	$(document).on('click','.escolas_ce',function()
    {
		var qrpid = $(this).attr('qrpid');
		var perid = $(this).attr('perid');
	    var param = '&req=formEscolasCe&qrpid='+qrpid+'&perid='+perid;
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
    			$('#html_modal-form').html(resp);
    		    $('#modal-form').modal();
       		}
     	});
	});

	$(document).on('click','.escolas_gremio',function()
    {
		var qrpid = $(this).attr('qrpid');
		var perid = $(this).attr('perid');
	    var param = '&req=formEscolasGremio&qrpid='+qrpid+'&perid='+perid;
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
    			$('#html_modal-form').html(resp);
    		    $('#modal-form').modal();
       		}
     	});
	});
});
</script>