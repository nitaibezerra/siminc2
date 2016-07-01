<?php

$controleUnidade         = new Par3_Controller_InstrumentoUnidade();
$controleUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();

$disabled = 'disabled';

if ($db->testa_superuser()) {
    $disabled = '';
}

$itrid  = $controleUnidade->pegarItrid($_REQUEST['inuid']);
$esfera = $controleUnidade->pegarDescricaoEsfera($_REQUEST['inuid']);

$testaPreenchimentoPrefeitura = $controleUnidadeEntidade->verificaPreencimentoPrefeitura($_REQUEST['inuid']);
$testaPreenchimentoPrefeito   = $controleUnidadeEntidade->verificaPreencimentoPrefeito($_REQUEST['inuid']);
$testaPreenchimentoSecretaria = $controleUnidadeEntidade->verificaPreencimentoSecretaria($_REQUEST['inuid']);
$testaPreenchimentoDirigente  = $controleUnidadeEntidade->verificaPreencimentoDirigente($_REQUEST['inuid']);
$testaPreenchimentoEquipe     = $controleUnidadeEntidade->verificaPreencimentoEquipe($_REQUEST['inuid']);

$iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
                           'style="color:green !important;'.
                                  'background-color: transparent;"></span>';

$iconPreenchimentoPrefeitura     = $iconNaoPreenchido;
$iconPreenchimentoPrefeito       = $iconNaoPreenchido;
$iconPreenchimentoSecretaria     = $iconNaoPreenchido;
$iconPreenchimentoDirigente      = $iconNaoPreenchido;
$iconPreenchimentoEquipe         = $iconNaoPreenchido;
$iconPreenchimentoPrefeitura     = $iconNaoPreenchido;
$iconPreenchimentoComite         = $iconNaoPreenchido;
$iconPreenchimentoNutricionistas = $iconNaoPreenchido;
$iconPreenchimentoConselho		 = $iconNaoPreenchido;

if ($testaPreenchimentoPrefeitura === 100)
    $iconPreenchimentoPrefeitura = $iconPreenchido;
if ($testaPreenchimentoPrefeito === 100)
    $iconPreenchimentoPrefeito = $iconPreenchido;
if ($testaPreenchimentoSecretaria === 100)
    $iconPreenchimentoSecretaria = $iconPreenchido;
if ($testaPreenchimentoDirigente === 100)
    $iconPreenchimentoDirigente = $iconPreenchido;
if ($testaPreenchimentoEquipe === 100)
    $iconPreenchimentoEquipe = $iconPreenchido;
if ($iconPreenchimentoComite === 100)
    $iconPreenchimentoComite = $iconPreenchido;
if ($iconPreenchimentoNutricionistas === 100)
    $iconPreenchimentoNutricionistas = $iconPreenchido;
if ($iconPreenchimentoConselho === 100)
	$iconPreenchimentoConselho = $iconPreenchido;

require APPRAIZ.'includes/cabecalho.inc';
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
                        <div class="col-md-3 colunaMenu">
	                        <?php if ($itrid === '2'): ?>
	                            <div class="menuUnidade <?php echo ( ($_REQUEST['menu'] == '' || $_REQUEST['menu'] === 'prefeitura') ? 'menuSelecionado' : '') ?>" tipo="prefeitura">
	                                <?php echo $iconPreenchimentoPrefeitura; ?>
	                                Prefeitura
	                            </div>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'prefeito' ? 'menuSelecionado' : '') ?>"
	                                 tipo="prefeito">
	                                <?php echo $iconPreenchimentoPrefeito; ?>
	                                Prefeito
	                            </div>
	                        <?php endif; ?>
                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'secretaria' ? 'menuSelecionado' : '') ?>" tipo="secretaria">
                                <?php echo $iconPreenchimentoSecretaria; ?>
                                Secretaria <?php echo $esfera; ?> de Educação
                            </div>
                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'dirigente' ? 'menuSelecionado' : '') ?>" tipo="dirigente">
                                <?php echo $iconPreenchimentoDirigente; ?>
                                <?php echo ($itrid === '2') ? 'Dirigente ' : 'Secretário '; ?>
                                <?php echo $esfera; ?> de Educação
                            </div>
                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'equipe' ? 'menuSelecionado' : '') ?>" tipo="equipe">
                                <?php echo $iconPreenchimentoEquipe; ?>
                                Equipe Local
                            </div>
                            <?php if ($itrid === '2'): ?>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'comite' ? 'menuSelecionado' : '') ?>" tipo="comite">
	                                <?php echo $iconPreenchimentoComite; ?>
	                                CACS
	                            </div>
	                        <?php endif; ?>
                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'nutricionista' ? 'menuSelecionado' : '') ?>" tipo="nutricionista">
                                <?php echo $iconPreenchimentoNutricionistas; ?>
                                Equipe Nutricionistas
                            </div>
                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'conselho' ? 'menuSelecionado' : '') ?>" tipo="conselho">
                                <?php echo $iconPreenchimentoConselho; ?>
                                Conselho <?php echo $esfera; ?> de Educação
                            </div>
                        </div>
                        <div class="col-md-9 colunaForm">
                            <?php $controleUnidade->desenharTela($_REQUEST['menu']); ?>
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
$(document).ready(function(){
    $('.menuUnidade').click(function(){

        var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
        var menu      = $(this).attr('tipo');
        var url       = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A'
                        +'&inuid=<?php echo $_REQUEST['inuid']?>&menu='+menu;

        if (menu != menuAtual) {
            $(location).attr('href',url);
        }

    });

    $('#entcpf').blur(function()
    {
    	if ($(this).val() != $('[name="entcpf_old"]').val()) {
       		$('input[name!="entcpf"][type=text]').val('');
    	}
    });
});
</script>