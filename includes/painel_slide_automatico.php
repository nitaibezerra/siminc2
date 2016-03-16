<?php

if($_POST['gerarGraficoPainelFinanceiro']){

    $sql = "select a.acaid, a.acadsc
            from painel.acaosistema asi
                inner join painel.acao a on a.acaid = asi.acaid
            where asi.sisid = {$_SESSION['sisid']}";
    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();

    $grafico = new Grafico();

    foreach ($dados as $dado) {
        $dadosOrcamentarios = getDadosOrcamentariosPorAcao($dado['acaid']);
        $dadosGrafico = array();
        foreach ($dadosOrcamentarios['total'] as $descricao => $aTotal) {
            foreach ($aTotal as $categoria => $valor) {
                $dadosGrafico[] = array('descricao' => $descricao, 'categoria' => $categoria, 'valor'=>$valor);
            }
        }

        echo '<br />';
        echo '<div class="tabela">';
        echo '<h3 style="color: #000; text-align: center; margin-bottom: 10px; border-bottom: #b3b3b3 solid 1px;">' . $dado['acadsc'] . '</h3>';
        echo '<div style="width: 50%; float: left;">';
        echo $grafico->setTipo(Grafico::K_TIPO_LINHA)->setHeight('200px')->gerarGrafico($dadosGrafico);
        echo '</div>';
        echo '<div style="width: 50%; float: left;">';
        echo $dadosOrcamentarios['tabela'];
//            echo $dadosOrcamentarios['tabela'];
        echo '</div>';
        echo '<div style="clear: both"></div>';
        echo '</div>';
    }
    exit;
}
if($_POST['gerarGraficoPainelIndicadores']){

    $sql = "select * from public.grafico where sisid = {$_SESSION['sisid']} order by graordem";
    $dadosGrafico = $db->carregar($sql);

    if ($dadosGrafico) {
        ?>
        <div>
            <?php $grafico = new Grafico();
            foreach($dadosGrafico as $aGrafico){
                $width = $aGrafico['gralargura'] ? $aGrafico['gralargura'] : 100;

                echo '<div style="width: ' . $width . '%; float: left">';

                if($aGrafico['gratipo']){
                    $grafico->setTipo($aGrafico['gratipo']);
                    if($aGrafico['gratipo'] == Grafico::K_TIPO_BARRA){
                        $grafico->setLabelX(array());
                    }
                }
                if($aGrafico['gratitulo']){
                    $grafico->setTitulo($aGrafico['gratitulo']);
                }
                if($aGrafico['grasubtitulo']){
                    $grafico->setSubtitulo($aGrafico['grasubtitulo']);
                }
                if($aGrafico['graaltura']){
                    $grafico->setHeight($aGrafico['graaltura']);
                }

//                setarParametrosGrafico($aGrafico, $grafico);
                $grafico->gerarGrafico($aGrafico['grasql']);
                echo '</div>';
            } ?>
        </div>
        <div style="clear: both;"></div>
    <?php
    }
    exit;
}

function montarPainelFinanceiro()
{ ?>
    <div id="container_painel_grafico_financeiro"></div>
    <script language="JavaScript">
        url = window.location.href;
        data = 'gerarGraficoPainelFinanceiro=true';
        jQuery.post(
            url
            , data
            , function(html){
                jQuery('#container_painel_grafico_financeiro').hide().html(html).fadeIn('slow');
            }
        );
    </script>
<?php
}

function montarPainelIndicadores()
{ ?>
    <div id="container_painel_grafico_indicadores"></div>
    <script language="JavaScript">
        url = window.location.href;
        data = 'gerarGraficoPainelIndicadores=true';
        jQuery.post(
            url
            , data
            , function(html){
                jQuery('#container_painel_grafico_indicadores').hide().html(html).fadeIn('slow');
            }
        );
    </script>
<?php
}