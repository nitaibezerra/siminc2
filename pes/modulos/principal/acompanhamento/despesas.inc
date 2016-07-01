<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<?php
include_once( APPRAIZ. "pes/classes/PesCelulaAcompanhamento.class.inc" );
include_once( APPRAIZ. "pes/classes/PesCelulaAcompNatDespesa.class.inc" );
include_once( APPRAIZ. "pes/modulos/principal/acompanhamento/_funcoes_despesas.php" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];
$unicod    = $_SESSION['pes']['codigo_unidadeorcamentaria'];

if ($_REQUEST['ajax']) {
    if ($_REQUEST['tidcodigo']) {

        if ($_REQUEST['tidcodigo'] == K_DESPESA_MATERIAL_CONSUMO) {
            $sql = "select cndcodigo as codigo, cndtitulo as descricao
                    from pes.pescontratonaturezadespesa
                    where entcodigo = '$entcodigo'
                    and tidcodigo = '{$_REQUEST['tidcodigo']}'
                    and natcodigo = '{$_REQUEST['natcodigo']}'
                    order by descricao";

            echo $db->monta_combo("concodigo",$sql,'S',"Selecione...","","","","200","S","concodigo","",$concodigo);
        } else {
            $sql = "select concodigo as codigo, contitulo as descricao
                    from pes.pescontrato
                    where entcodigo = '$entcodigo'
                    and tidcodigo = '{$_REQUEST['tidcodigo']}'
                    order by descricao";

            echo $db->monta_combo("concodigo",$sql,'S',"Selecione...","","","","200","S","concodigo","",$concodigo);
        }
    }
    die;
}

// Todos os tipos de despesa, exceto materiais de consumo e despesas genï¿½ricas
if (is_array($_POST['ceavalor'])) {
    try {

        $totalInicial = recuperarTotalDespesaEntidadeMes($_POST['tidcodigo'], $entcodigo);

        foreach ($_POST['ceavalor'] as $ceaano => $aConfig) {
            $sql = "delete from pes.pescelulaacompanhamento
                    where concodigo = '{$_POST['concodigo']}'
                    and ceaano = '$ceaano'";
            $db->executar($sql);

            foreach ($aConfig as $ccdcodigo => $ceavalor) {

                $modelAcompanhamento = new PesCelulaAcompanhamento();

                $modelAcompanhamento->ceaano    = $ceaano;
                $modelAcompanhamento->ceavalor  = $ceavalor ? real2Db($ceavalor) : null;
                $modelAcompanhamento->ccdcodigo = $ccdcodigo;
                $modelAcompanhamento->concodigo = $_POST['concodigo'];
                $modelAcompanhamento->salvar();
            }
        }

        $db->Commit();
        $totalFinal = recuperarTotalDespesaEntidadeMes($_POST['tidcodigo'], $entcodigo);

        verificarDiferencaValores($entcodigo, $totalInicial, $totalFinal);

        alertlocation(array(
            'alert'=>MSG003,
            'location'=>"$url&concodigo={$_POST['concodigo']}&seriehistorica={$_REQUEST['seriehistorica']}&tidcodigo={$_REQUEST['tidcodigo']}"
        ));
    } catch (Exception $e) {
        $db->Rollback();
        alertlocation(array(
            'alert'=>MSG004,
            'location'=>"$url&concodigo={$_POST['concodigo']}&seriehistorica={$_REQUEST['seriehistorica']}&tidcodigo={$_REQUEST['tidcodigo']}"
        ));
    }
}

//$aEmail[] = array('mes'=>7, 'ano'=>2013, 'valorInicial'=>1000, 'valorFinal'=>2000);
//$aEmail[] = array('mes'=>5, 'ano'=>2013, 'valorInicial'=>'', 'valorFinal'=>15000);
//        enviarEmailAlteracaoValores($aEmail, $entcodigo);

// Salvar materiais de consumo
if (is_array($_POST['canvalor'])) {

    try {

        $totalInicial = recuperarTotalDespesaEntidadeMes($_POST['tidcodigo'], $entcodigo);

        foreach ($_POST['canvalor'] as $canano => $aMes) {
            $sql = "delete from pes.pescelulaacompnatdespesa
                    where cndcodigo = '{$_POST['concodigo']}'
                    and canano = '$canano'";
            $db->executar($sql);

            foreach ($aMes as $canmes => $aTipo) {
                foreach ($aTipo as $cantipovalor => $canvalor) {

                    $modelAcompNatDespesa = new PesCelulaAcompNatDespesa();
                    $modelAcompNatDespesa->canano       = $canano;
                    $modelAcompNatDespesa->canmes       = $canmes;
                    $modelAcompNatDespesa->cantipovalor = $cantipovalor;
                    $modelAcompNatDespesa->canvalor     = $canvalor ? real2Db($canvalor) : null;
                    $modelAcompNatDespesa->cndcodigo    = $_POST['concodigo'];

                    $modelAcompNatDespesa->salvar();
                }
            }
        }

        $db->Commit();

        $totalFinal = recuperarTotalDespesaEntidadeMes($_POST['tidcodigo'], $entcodigo);

        verificarDiferencaValores($entcodigo, $totalInicial, $totalFinal);

        alertlocation(array(
            'alert'=>MSG003,
            'location'=>"$url&concodigo={$_POST['concodigo']}&seriehistorica={$_REQUEST['seriehistorica']}&tidcodigo={$_REQUEST['tidcodigo']}&natcodigo={$_REQUEST['natcodigo']}"
        ));
    } catch (Exception $e) {
        $db->Rollback();
        alertlocation(array(
            'alert'=>MSG004,
            'location'=>"$url&concodigo={$_POST['concodigo']}&seriehistorica={$_REQUEST['seriehistorica']}&tidcodigo={$_REQUEST['tidcodigo']}&natcodigo={$_REQUEST['natcodigo']}"
        ));
    }
}

extract($_REQUEST);
$concodigo      = $concodigo      ? $concodigo      : 0;
$tidcodigo      = $tidcodigo      ? $tidcodigo      : 0;
$aexano         = $aexano         ? $aexano         : AEXANO;
$seriehistorica = $seriehistorica ? $seriehistorica : AEXANO - 1;

if ($tidcodigo == K_DESPESA_COLETA_SELETIVA) {
    $seriehistorica = AEXANO;

    $sql = "select concodigo from pes.pescontrato
            where tidcodigo = " . K_DESPESA_COLETA_SELETIVA . "
            and conano = '$aexano'
            and entcodigo = $entcodigo
            limit 1";

    $concodigo = $db->pegaUm($sql);

    if(!$concodigo){
        $sql = "insert into pes.pescontrato(
                        entcodigo, tidcodigo, contitulo, conano, conusucpfcriacao)
                values ($entcodigo, " . K_DESPESA_COLETA_SELETIVA . ", 'CSS{$aexano}', '{$aexano}', '{$_SESSION['usucpforigem']}')
                returning concodigo";

        $concodigo = $db->pegaUm($sql);
        $db->commit();

    }
}

include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
print '<br/>';

//ver($abacod_tela, $url);

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );



$controllerGeral = new Controller_Geral();
if($controllerGeral->permission() < 3)
    $save = 'S';
else
    $save = 'N';

?>

<style type="text/css">
    .colunaDestaque{
        background-color: #E3E3E3;
        border-right: 1px solid #c0c0c0;
        border-bottom: 1px solid #c0c0c0;
        border-left: 1px solid #ffffff;
    }
</style>
<form action="" method="post" name="formulario-busca" id="formulario-busca">
    <input type="hidden" name="atiid" value="<?= $atiid ?>"/>
    <input type="hidden" name="gravar" value="1"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Despesa:</td>
            <td>
                <?php
                $sql= "select tidnome as descricao, tidcodigo as codigo
                       from  pes.pestipodespesa
                       where tidcodigo not in (" . K_DESPESA_DIARIAS . ", " . K_DESPESA_PASSAGENS . ", " . K_DESPESA_GENERICA . ")
                       order by tidordem desc, descricao";
                echo $db->monta_combo("tidcodigo",$sql,'S',"Selecione...","","","","200","S","tidcodigo","",$tidcodigo);
                ?>
            </td>
        </tr>
        <tr class="exibir-info exibir-material-consumo">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Subelemento:</td>
            <td>
                <?php
                    $sql= "select natcodigo || ' - ' || natdescricao as descricao, natcodigo as codigo
                           from pes.pesnaturezadespesa
                           where natcodigopai = '33903000'
                           order by natdescricao";

                    echo $db->monta_combo("natcodigo",$sql,'S',"Selecione...","","","","200","S","natcodigo","",$natcodigo);
                ?>
            </td>
        </tr>
        <tr class="exibir-info">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Contrato:</td>
            <td>
                <span id="span-contrato">
                    <?php

                    if ($tidcodigo == K_DESPESA_MATERIAL_CONSUMO) {
                        $sql= " select cndcodigo as codigo, cndtitulo as descricao
                                from pes.pescontratonaturezadespesa
                                where entcodigo = '$entcodigo'
                                and tidcodigo = '$tidcodigo'
                                and natcodigo = '{$natcodigo}'
                                order by descricao";

                        echo $db->monta_combo("concodigo",$sql,'S',"Selecione...","","","","200","S","concodigo","",$concodigo);
                    } else {

                        $sql= " select concodigo as codigo, contitulo as descricao
                                from pes.pescontrato
                                where entcodigo = '$entcodigo'
                                and tidcodigo = '$tidcodigo'
                                order by descricao";

                        echo $db->monta_combo("concodigo",$sql,'S',"Selecione...","","","","200","S","concodigo","",$concodigo);
                    }
                    ?>
                </span>
                <?php
                $controller = new Controller_Geral();
                if( $controller->permission() < 3): ?>
                <input type="button" name="botao_contrato" id="botao_contrato" value="Novo Contrato" href="pes.php?modulo=principal/contrato/contrato&acao=A" />
                <?php endif ?>
            </td>
        </tr>
        <tr class="exibir-info">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Série Histórica de:</td>
            <td>
                    <?php
                    $sql= " select aexano as codigo, aexano as descricao
                            from pes.pesanoexercicio
                            where aexano != '$aexano'
                            order by descricao";

                    echo $db->monta_combo("seriehistorica",$sql,'S',"","","","","100","S","","",$seriehistorica);
                    ?>
            </td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_buscar" id="botao_buscar" value="Buscar" />
            </td>
        </tr>
    </table>
</form>

<?php if ($concodigo) { ?>

    <?php

        // Anos a exibir (obs: ha casos que nao há serie, logo os dois itens serao o ano corrente, motivo este do uso do array_unique)
        $anos = array_unique(array($seriehistorica, $aexano));

        // Configuraï¿½ï¿½es por tipo de despesa
        $sql = "select * from pes.pesconfigcontratodespesa ccd
                    inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                    inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                where ccdtipoconfig = 'CA'
                and tidcodigo = $tidcodigo
                and substring(cconome from 1 for 1) != '[' -- Pegando os dados sem colchetes (acompanhamento avanï¿½ado)
                order by lcoordem, ccototaliza, ccoordem";

        $configuracao = $db->carregar($sql);

        $aConfigs = array();
        if($configuracao){
            foreach ($configuracao as $config) {
                $configs[$config['ccocodigo']] = $config['cconome'];
                $aConfigs[$config['lcocodigo'] . ' - ' . $config['lconome']][$config['ccocodigo']] = $config;
            }
        }

//      ver($aConfigs);

        // Dados de acompanhamento gravados no banco
        $sql = "select * from pes.pescelulaacompanhamento cea
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                where ceaano in ($seriehistorica, $aexano)
                and concodigo = $concodigo
                and ccdtipoconfig = 'CA'";

        $acompanhamento = $db->carregar($sql);
        $aAcompanhamento = array();
        foreach ((array)$acompanhamento as $dado) {
            $aAcompanhamento[$dado['lcocodigo']][$dado['ceaano']][$dado['ccocodigo']] = $dado;
        }
    ?>

    <?php if($aConfigs || ($tidcodigo == K_DESPESA_PROCESSAMENTO_DADOS) || ($tidcodigo == K_DESPESA_TELECOMUNICACOES) || ($tidcodigo == K_DESPESA_MATERIAL_CONSUMO)) { ?>
<?php if($save == 'S'): ?>
        <form action="" name="formulario_gravar"  id="formulario_gravar" method="post">
            <input type="hidden" name="concodigo" value="<?php echo $concodigo; ?>"/>
            <input type="hidden" name="tidcodigo" value="<?php echo $tidcodigo; ?>"/>
            <input type="hidden" name="natcodigo" value="<?php echo $natcodigo; ?>"/>
            <input type="hidden" name="seriehistorica" id="seriehistorica" value="<?php echo $seriehistorica; ?>"/>
<?php endif ?>

            <?php switch($tidcodigo){
                case (K_DESPESA_PROCESSAMENTO_DADOS): $aConfigs = montarAcompanhamentoProcessamentoDados($aConfigs, $anos, $configs, $aAcompanhamento, $aexano); break;
                case (K_DESPESA_TELECOMUNICACOES)   : $aConfigs = montarAcompanhamentoTelecomunicacoes($aConfigs, $anos, $configs, $aAcompanhamento, $aexano);   break;
                case (K_DESPESA_MATERIAL_CONSUMO)   : montarAcompanhamentoMaterialConsumo($concodigo, $anos, $aexano, $entcodigo, $unicod);                      break;
                default                             : montarAcompanhamentoGeral($aConfigs, $anos, $configs, $aAcompanhamento, $aexano);                          break;
            } ?>
<?php if($save == 'S'): ?>
        </form>
<?php endif ?>

        <?php


        $aTidcodigoConcolidadoGeral = array(
            K_DESPESA_AGUA_ESGOTO, K_DESPESA_APOIO_ADM, K_DESPESA_ENERGIA_ELETRICA, K_DESPESA_LIMPEZA,
            K_DESPESA_LOCACAO_IMOVEIS, K_DESPESA_LOCACAO_VEICULOS, K_DESPESA_MANUTENCAO_BENS, K_DESPESA_VIGILANCIA,
            K_DESPESA_PROCESSAMENTO_DADOS, K_DESPESA_TELECOMUNICACOES,
        );

        if (in_array($tidcodigo, $aTidcodigoConcolidadoGeral)) {
            montaTabelaConsolidadoAlunoEquivalente($unicod, $entcodigo, $tidcodigo);
            montarConsolidadoSOF($entcodigo, $unicod, $tidcodigo, $aConfigs);
        }

    } else {
        echo '<h1 style="color: red; text-align: center;">Não foi encontrada a configuração para a despesa escolhida.</h1>';
    }
} ?>

<script type="text/javascript">
    jQuery(function(){

        if(jQuery('#tidcodigo').val() == '<?php echo K_DESPESA_COLETA_SELETIVA; ?>'){
            jQuery('.exibir-info').hide();
        } else{
            if(jQuery('#tidcodigo').val() == '<?php echo K_DESPESA_MATERIAL_CONSUMO; ?>'){
                jQuery('.exibir-material-consumo').show();
            } else {
                jQuery('.exibir-material-consumo').hide();
            }
        }


        jQuery('#tidcodigo').change(function(){
            if(jQuery(this).val() == '<?php echo K_DESPESA_COLETA_SELETIVA; ?>'){
                jQuery('.exibir-info').hide();
            } else {
                jQuery('.exibir-info').show();
                if(jQuery(this).val() != '<?php echo K_DESPESA_MATERIAL_CONSUMO; ?>'){
                    jQuery('.exibir-material-consumo').hide();
                } else {
                    jQuery('.exibir-material-consumo').show();
                }
                jQuery('#span-contrato').load('pes.php?modulo=principal/acompanhamento/despesas&acao=A&ajax=1&tidcodigo=' + $(this).val());
            }
        });

        jQuery('#natcodigo').change(function(){
            jQuery('#span-contrato').load('pes.php?modulo=principal/acompanhamento/despesas&acao=A&ajax=1&tidcodigo=12&natcodigo=' + $(this).val());
        });

        jQuery('#botao_buscar').click(function(){
            if(!jQuery('#tidcodigo').val()){
                alert('O campo [Despesa] é obrigatório.');
                return false;
            }

            if(!jQuery('#concodigo').val() && (jQuery('#tidcodigo').val() != '<?php echo K_DESPESA_COLETA_SELETIVA; ?>')){
                alert('O campo [Contrato] é obrigatório.');
                return false;
            }

            jQuery('#formulario-busca').submit();
        });

        jQuery('.botao_gravar').click(function(){
            jQuery('#formulario_gravar').submit();
        });

        jQuery('#botao_contrato').click(function(){
            window.location.href = jQuery(this).attr('href') + '&tidcodigo=' + jQuery('#tidcodigo').val() + '&natcodigo=' + jQuery('#natcodigo').val();
        });

        jQuery('.soma').keyup(function(){
            var coluna = $(this).attr('coluna');
            atualizaTotal('soma_'+coluna, 'span_'+coluna);
        }).keyup();

        jQuery('.linha').keyup(function(){
            var linha = $(this).attr('linha');
            atualizaTotal('linha_'+linha, 'span_'+linha);

            var ano = $(this).attr('ano');
            atualizaTotal('linha_'+ano, 'span_total_'+ano);
        }).keyup();

        jQuery('.parcial').keyup(function(){
            var parcial = $(this).attr('parcial');

            var valor1 = jQuery('.' + parcial).eq(0).val() ? str_replace(['.', ','], ['', '.'], jQuery('.' + parcial).eq(0).val()) : 0;
            var valor2 = jQuery('.' + parcial).eq(1).val() ? str_replace(['.', ','], ['', '.'], jQuery('.' + parcial).eq(1).val()) : 0;
            var resultado = parseFloat(valor1) * parseFloat(valor2);

            jQuery('#'+parcial).val(number_format(resultado, 2, ',', '.')).keyup();

        }).keyup();
    });
</script>
