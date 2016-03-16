<?php
ob_start();

global $db;

// --------------- Dependências
include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/RelatorioAvaliadorRedeae.class.inc';

$usucpf = $_SESSION['usucpf'];
$diretoria = isset($_GET['diretoria']) ? $_GET['diretoria'] : 1;

switch ($_REQUEST['acao']){
    case 'novo':
        $url = validaUrlPerfil('RelatorioAvaliador', null, $diretoria);
        //echo "<script>window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';</script>";
        echo "<script>window.location.href = '{$url}';</script>";
        exit;
        break;
    case 'download':
        ob_clean ();

        $rel = new RelatorioAvaliadorRedeae();
        $rel->carregarPorId($_REQUEST['ravid']);
        $html = '';
        $nome = $rel->pegaAvaliador();
        switch ($rel->arAtributos['pflcod']){
            case PFLCOD_SASE_SUPERVISOR_GERAL:
            case PFLCOD_SASE_EXECUTIVO:
                $html = $rel->geraRelatorioSuperGeral();
                break;

            case PFLCOD_SASE_TECNICO:
            case PFLCOD_SASE_SUPERVISOR:
                $html = $rel->geraRelatorio();
                break;
        }

        $content = http_build_query(array(
            'conteudoHtml' => utf8_encode($html)
        ));

        $context = stream_context_create(array(
            'http' => array (
                'method' => 'POST',
                'content' => $content
            )
        ));

        $contents = file_get_contents ( 'http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
        header ( 'Content-Type: application/pdf' );
        header ( 'Content-Disposition: attachment; filename=Relatorio_AE_'.$nome.'_'.$_REQUEST['ravid']);
        echo $contents;
        exit();
        break;

    case 'apaga_relatorio':
        $ravid = $_REQUEST['ravid'];
        $rel = new RelatorioAvaliadorRedeae();
        $rel->carregarPorId($ravid);
        if ($rel->apagaRelatorio()) {
            echo "
					<script>
						alert('Relatório apagado com sucesso.');
						window.location.href = 'sase.php?modulo=principal/regrelatorioplanospne&acao=A';
					</script>";
            exit;
        } else {
            echo "
					<script>
						alert('{$rel->msg}');
						window.location.href = 'sase.php?modulo=principal/regrelatorioplanospne&acao=A';
					</script>";
        }
        break;

    case 'carrega_avaliador':
        $pflcod = $_POST['pflcod'];
        if ($pflcod != '') {
            $sql = "select distinct
                    ave.aveid as codigo,
                    usu.usunome as descricao
                from sase.avaliadoreducacional ave
                inner join seguranca.usuario usu on ave.avenumcpf = usu.usucpf
                inner join seguranca.perfilusuario pfu on pfu.usucpf = usu.usucpf
                inner join seguranca.perfil pfl
                    on pfu.pflcod = pfl.pflcod
                    and sisid = " . SASE_SISID . "
                    and pflstatus = 'A'
                where ave.avestatus = 'A'
                ave.avediretoria = {$diretoria}
                and pfl.pflcod = {$pflcod}
                order by usu.usunome";
        } else {
            $sql = "select
                        ave.aveid as codigo,
                        usu.usunome as descricao
                    from sase.avaliadoreducacional ave
                    inner join seguranca.usuario usu on ave.avenumcpf = usu.usucpf
                    where ave.avediretoria = {$diretoria}
                    order by usu.usunome";

        }

        $dados = $db->carregar($sql);

        echo getOptions( $dados, array('prompt' => ' Selecione...'), 'aveid', null, $_REQUEST['aveid']);
        exit();

    case 'exportar-excel':
        $rel = new RelatorioAvaliadorRedeae();
        $rel->recuperarListagem('xls');
        exit();

}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
//include APPRAIZ . 'includes/funcoes.inc';
//include APPRAIZ . 'www/sase/_funcoes.php';
// --

$avaliadoreducacional = new AvaliadorEducacional();

$rel = new RelatorioAvaliadorRedeae();

$rel->arAtributos['usucpf'] = $usucpf;

$per = $rel->retornaPermissao();

$data = new Data();

include APPRAIZ . 'includes/cabecalho.inc';

$titulo = 'Relatório dos Resultados da Adequação';

$ravid = $_GET['ravid'];

if ($ravid != ''){
    $rel->carregarPorId($ravid);
    //ver($rel, d);
    $dados = $rel->getDados();
}

if (!$_REQUEST['aveid']){
    $sql = "select aveid, estuf from sase.avaliadoreducacional where avenumcpf = '".$_SESSION['usucpf']."'";
    $res = $db->carregar($sql);
    $aveid = $res['aveid'];
    $estuf = $res['estuf'];
}

$usucpf = $rel->arAtributos['usucpf'];
$ratperiodo = $rel->arAtributos['ratperiodo'];
$ratdata1periodo = $data->formataData($rel->arAtributos['ratdata1periodo']);
$ratdata2periodo = $data->formataData($rel->arAtributos['ratdata2periodo']);


?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>
    function submitForm(request){
        if (request !== 'undefined'){
            jQuery('[name=acao]').val(request);
            jQuery('[name=formCadastroLista]').submit();
        } else {
            alert('Ação não informada.');
        }
    }

    function apagarRelatorio(ravid){
        console.log(ravid);
        if (ravid != ''){
            if (confirm('Deseja apagar este relatório?')) {
                jQuery('[name=acao]').val('apaga_relatorio');
                jQuery('[name=ravid]').val(ravid);
                jQuery('[name=formCadastroLista]').submit();
            }
        } else {
            console.error('Variável \'ravid\' sem valor.');
        }
    }

    function carregaAvaliador(selpfl, id){
        divCarregando();
        jQuery.ajax({
            type: 'POST',
            url: 'sase.php?modulo=principal/regrelatorioplanospne&acao=A',
            data: {acao: 'carrega_avaliador', pflcod: selpfl, id: id},
            async: false,
            success: function(data){
                $('#divAvaliador').html(data);
                divCarregado();
            }
        });
    }

    function editRelatorio(id, ravid, pflcod){
        var url = '';

        <?php
        if(isset($_GET['diretoria']) && $_GET['diretoria'] == 2){ ?>
        url = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria=2&ravid='+ravid;
        if(pflcod == <?= PFLCOD_SASE_TECNICO_DIVAPE ?>){
            url = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria=2&ravid='+ravid;
        }
        <?php } else { ?>
        url = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid='+ravid;
        if (pflcod == <?= PFLCOD_SASE_SUPERVISOR_GERAL ?>){
            url = 'sase.php?modulo=relatorio/relResultPESuperGeral&acao=A&ravid='+ravid;
        }
        if (pflcod == <?= PFLCOD_SASE_EXECUTIVO ?>){
            url = 'sase.php?modulo=relatorio/relResultPECoordenador&acao=A&ravid='+ravid;
        }
        <?php } ?>
        window.location.href = url;
    }

    function downloadRelatorio(ravid){
        jQuery('[name=acao]').val('download');
        jQuery('[name=ravid]').val(ravid);
        jQuery('[name=formCadastroLista]').submit();
    }
</script>

<div id='cabecalhoSistema'>
	<center><h3><?=$titulo?></h3></center>
</div>

<!-- html -->
<div id="container">
	<div class="row">
		<div class="row col-md-12">
			<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
				<input type="hidden" id="requisicao" name="requisicao" value="" />
				<input name="acao" value="carregar" type="hidden"/>
                <input name="ravid" value="" type="hidden"/>
                <input name="usucpf" value="<?=@$usucpf?>" type="hidden"/>
								
				<div class="row col-md-12">
					<div class="well">
						<fieldset>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Perfil</label>
                                <div class="col-lg-8 col-md-8">
                                    <?php
                                    $sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflcod in (".PFLCOD_SASE_TECNICO.",".PFLCOD_SASE_SUPERVISOR.",".PFLCOD_SASE_SUPERVISOR_GERAL.",".PFLCOD_SASE_EXECUTIVO.") order by pfldsc";
                                    $dados = $db->carregar($sql);
                                    echo getoptions($dados, array('prompt' => ' Selecione...'), 'pflcod', 'carregaAvaliador(this.value, \'pflcod\')', $_REQUEST['pflcod']);
                                    ?>
                                </div>
                            </div>
							<?php if ($per > 1){ ?>
							<div class="form-group">
							    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Avaliador Educacional:</label>
							
							    <div class="col-lg-8 col-md-8 ">
                                    <div id="divAvaliador">
                                        <?php
                                        $sql = "select distinct
                                                    ave.aveid as codigo,
                                                    usu.usunome as descricao
                                                from sase.avaliadoreducacional ave
                                                inner join seguranca.usuario usu on ave.avenumcpf = usu.usucpf
                                                where ave.avediretoria = {$diretoria}
                                                order by usu.usunome";

                                        $dados = $db->carregar($sql);

                                        echo getOptions( $dados, array('prompt' => ' Selecione...'), 'aveid', null, $_REQUEST['aveid']);
                                        ?>
                                    </div>
				               	</div>
							</div>
                                <div class="form-group">
                                    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">UF:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <div id="divAvaliador">
                                            <?php
                                            $sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";

                                            $dados = $db->carregar($sql);

                                            echo getOptions( $dados, array('prompt' => ' Selecione...'), 'estuf', null, $_REQUEST['estuf']);
                                            ?>
                                        </div>
                                    </div>
                                </div>
							<?php } else { ?>
                                <input type="hidden" name="aveid" value="<?= $aveid ?>"/>
                                <input type="hidden" name="estuf" value="<?= $estuf ?>"/>
                            <?php } ?>
                            <div class="form-group">
                                <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Situação do Relatório:</label>

                                <div class="col-lg-8 col-md-8 ">
                                    <div id="divAvaliador">
                                        <?php
                                        $sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = ".TPDID_SASE_AVALIADOREDUCACIONAL." order by esdordem";

                                        $dados = $db->carregar($sql);

                                        echo getOptions( $dados, array('prompt' => ' Selecione...'), 'esdid', null, $_REQUEST['esdid']);
                                        ?>
                                    </div>
                                </div>
                            </div>
							<div class="form-group">
							    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período em data:</label>
							
							    <div class="col-lg-8 col-md-8 ">
							    	<table>
							    		<tr>
							    			<td><input id="ratdata1periodo" name="ratdata1periodo" value="<?= $_REQUEST['ratdata1periodo'] ?>" type="text" class="campoData form-control" placeholder=""></td>
							    			<td>à</td>
							    			<td><input id="ratdata2periodo" name="ratdata2periodo" value="<?= $_REQUEST['ratdata2periodo'] ?>" type="text" class="campoData form-control" placeholder=""></td>
							    		</tr>
							    	</table>
				               </div>
							</div>
							
							<div class="text-right">
                                <button title="Salvar" class="btn btn-primary" type="button" id="btnPesquisar" onclick="submitForm('novo')">
                                    Novo
                                </button>
								<button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="submitForm('pesquisar')">Pesquisar
								</button>
                                <button title="Salvar" class="btn btn-warning" type="button" id="btnPesquisar" onclick="submitForm('exportar-excel')">Exportar XLS
                                </button>
							</div>
														
						</fieldset>
					</div>
				</div>


			</form>
            <div>
                <?php $rel->recuperarListagem('rel', $diretoria); ?>
            </div>
		</div>
	</div>
</div>
<script>
$(document).ready(function(){
	$('.campoCpf').mask("999.999.999-99");
    $('.campoData').mask('99/99/9999');
    $('.campoData').datepicker();

	$('.obrigatorio').append('<span style="color: red;">*</span>');
    
});
</script>
							