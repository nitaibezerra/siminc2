<?php
include_once APPRAIZ . "includes/library/simec/Grafico.php";
global $db;

if($_REQUEST['gravar']) {

    $grasql = str_replace(array('\\', "'"), array('', "''"), $_REQUEST['grasql']);
    $gratitulo = $_REQUEST['gratitulo'] ? "'{$_REQUEST['gratitulo']}'" : 'null';
    $grasubtitulo = $_REQUEST['grasubtitulo'] ? "'{$_REQUEST['grasubtitulo']}'" : 'null';
    $graaltura = $_REQUEST['graaltura'] ? $_REQUEST['graaltura'] : 'null';
    $gralargura = $_REQUEST['gralargura'] ? $_REQUEST['gralargura'] : 'null';
    $graordem = $_REQUEST['graordem'] ? $_REQUEST['graordem'] : 'null';

    if($_REQUEST['graid']){
        $sql = "update public.grafico set
                    sisid = {$_REQUEST['sisid']},
                    grasql = '$grasql',
                    gratipo = '{$_REQUEST['gratipo']}',
                    gratitulo = $gratitulo,
                    grasubtitulo = $grasubtitulo,
                    graaltura = $graaltura,
                    gralargura = $gralargura,
                    graordem = $graordem
                where graid = {$_REQUEST['graid']}
        ";
        $db->executar($sql);

        $graid = $_REQUEST['graid'];
    } else {
        $sql = "INSERT INTO public.grafico(sisid, grasql, gratipo, gratitulo, grasubtitulo, graaltura, gralargura, graordem)
                            VALUES ({$_REQUEST['sisid']}, '$grasql', '{$_REQUEST['gratipo']}', $gratitulo, $grasubtitulo, $graaltura, $gralargura, $graordem) returning graid";

        $graid = $db->pegaUm($sql);
    }
    $db->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location.href = '$url';
        </script>");
}
?>

<style>
    .label_campo{
        margin-left: 2px;
        margin-right: 10px;
    }
</style>

<?php

if($_REQUEST['graid']){
    $sql = "select * from public.grafico where graid = {$_REQUEST['graid']}";
    $dadosGrafico = $db->carregar($sql);
    if(is_array($dadosGrafico)){
        $aGrafico = current($dadosGrafico);
    }
} else {
    $sql = "select * from public.grafico where sisid = {$_SESSION['sisid']} order by graordem";
    $dadosGrafico = $db->carregar($sql);
}

include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>
<form action="" method="post" name="formulario" id="formulario">
    <input type="hidden" name="graid" id="graid" value="<?php echo $aGrafico['graid']; ?>"/>
    <input type="hidden" name="gravar"  value="1"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Módulo:</td>
            <td>
                <?php
                $sql = "select sisid as codigo, sisdsc || ' (' || sisabrev || ')' as descricao
                        from seguranca.sistema
                        where sisid = '{$_SESSION['sisid']}'
                        order by descricao";

                $sisid = $modelMensagemRegra->sisid;
                echo $db->monta_combo("sisid",$sql,'S',"Selecione...","","","","400","S","sisid","",$aGrafico['sisid']);
                ?>
    </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="width:25%;">Tipo:</td>
        <td>
            <?php
            $dados = array(
                array('codigo'=>'A', 'descricao'=>'Área'),
                array('codigo'=>'B', 'descricao'=>'Barra'),
                array('codigo'=>'C', 'descricao'=>'Coluna'),
                array('codigo'=>'L', 'descricao'=>'Linha'),
                array('codigo'=>'P', 'descricao'=>'Pizza'),
            );

            echo $db->monta_combo("gratipo",$dados,'S',"Selecione...","","","","400","S","","",$aGrafico['gratipo']);
            ?>
        </td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">Título:</td>
        <td><?= campo_texto('gratitulo', 'N', 'S', '', 60, 100, '', '', '', '', '', '', '',$aGrafico['gratitulo']); ?></td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">Subtítulo:</td>
        <td><?= campo_texto('grasubtitulo', 'N', 'S', '', 60, 100, '', '', '', '', '', '', '',$aGrafico['grasubtitulo']); ?></td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">Altura (px):</td>
        <td><?= campo_texto('graaltura', 'N', 'S', '', 5, 10, '', '', '', '', '', '', '',$aGrafico['graaltura']); ?></td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">Largura (%):</td>
        <td><?= campo_texto('gralargura', 'N', 'S', '', 5, 10, '', '', '', '', '', '', '',$aGrafico['gralargura']); ?></td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">Ordem:</td>
        <td><?= campo_texto('graordem', 'N', 'S', '', 5, 10, '', '', '', '', '', '', '',$aGrafico['graordem']); ?></td>
    </tr>
    <tr >
        <td align='right' class="SubTituloDireita" style="width:25%;">SQL:</td>
        <td><?= campo_textarea('grasql', 'N', 'S', '', 50, 10, '', '', null, null, null, null, $aGrafico['grasql'], '800px'); ?></td>
    </tr>
    <tr id="tr_botoes_acao" style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
        <td>
            <input type="button" name="botao_enviar" id="botao_enviar" value="Salvar" />
            <input type="button" name="botao_voltar" id="botao_voltar" value="Voltar" />
        </td>
    </tr>
    </table>
</form>

<?php if (is_array($dadosGrafico) && count($dadosGrafico)) { ?>
    <div>
        <h3 style="text-align: center;">Visualização</h3>
        <?php $grafico = new Grafico();
        foreach($dadosGrafico as $aGrafico){
            $width = $aGrafico['gralargura'] ? $aGrafico['gralargura'] : 100;

            echo '<div style="width: ' . $width . '%; float: left">
                <a href="' . $url . '&graid=' . $aGrafico['graid'] . '" >Alterar Gráfico</a>
                ';
                setarParametrosGrafico($aGrafico, $grafico);
                $grafico->gerarGrafico($aGrafico['grasql']);
            echo '</div>';
        } ?>
    </div>
<?php } ?>


<script type="text/javascript">
    $1_11(function(){
        $1_11('#botao_enviar').click(function(){
            $1_11('#formulario').submit();

        });
    });        
</script>

<?php
function setarParametrosGrafico($dadosGrafico, $grafico){
    if($dadosGrafico['gratipo']){
        $grafico->setTipo($dadosGrafico['gratipo']);
        if($dadosGrafico['gratipo'] == Grafico::K_TIPO_BARRA){
            $grafico->setLabelX(array());
        }
    }
    if($dadosGrafico['gratitulo']){
        $grafico->setTitulo($dadosGrafico['gratitulo']);
    }
    if($dadosGrafico['grasubtitulo']){
        $grafico->setSubtitulo($dadosGrafico['grasubtitulo']);
    }
    if($dadosGrafico['graaltura']){
        $grafico->setHeight($dadosGrafico['graaltura']);
    }
}
?>
