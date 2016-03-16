<?php
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_LISTAR_PROFIS, ABA_FP_FORM_PROFISSIONAL, ABA_FP_IMPORTAR_ARQUIVO);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo( 'Complementação do Piso Salarial', 'Folha de Pagamento' );
montaCabecalho();

$arAnos = array('2009'=>'2009',
                '2010'=>'2010',
                '2011'=>'2010');

foreach($arAnos as $k => $v){
    unset($_SESSION['rsJornadaAlunos'.$k],$_SESSION['rsAP'.$k],$_SESSION['rsAPAD'.$k],$_SESSION['rsAPAE'.$k]);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function(){

    $('.analisar').click(function(){

        manutencao = $('input[name=manutencao]:checked').val();
        financas   = $('input[name=financas]:checked').val();
        siope      = $('input[name=siope]:checked').val();
        muncod     = $('input[name=muncod]').val();
        parm       = $('input[name=parm]').val();
        pafr       = $('input[name=pafr]').val();
        pmde       = $('input[name=pmde]').val();
        iflei      = $('input[name=iflei]').val();

        if(!manutencao){
            alert('Campo manutenção obrigatório!');
            $('input[name=manutencao]').focus();
            return false;
        }
        if(!financas){
            alert('Campo finanças obrigatório!');
            $('input[name=financas]').focus();
            return false;
        }
        if(!siope){
            alert('Campo SIOPE obrigatório!');
            $('input[name=siope]').focus();
            return false;
        }

        if(manutencao == 'N' || financas == 'N' || siope == 'N'){
            alert('Todos os campos devem ser sim!');
            return false;
        }

        if(parm == ''){
            alert('O campo PARM é obrigatório!');
            $('input[name=parm]').focus();
            return false;
        }

        if(pafr == ''){
            alert('O campo PAFR é obrigatório!');
            $('input[name=pafr]').focus();
            return false;
        }

        if(pmde == ''){
            alert('O campo PMDE é obrigatório!');
            $('input[name=pmde]').focus();
            return false;
        }

        if(iflei == ''){
            alert('O campo IFLEI é obrigatório!');
            $('input[name=iflei]').focus();
            return false;
        }

        $.ajax({
            url: 'ajax.php',
            type: 'post',
            data: 'requisicao=verificaMunicipioAnalise&muncod='+muncod,
            success: function(e){
                if(e == 'true'){
                    $('#formAnalise').attr('action','pisosalarial.php?modulo=principal/folhapagamento/analise/resultadoGeral&acao=A');
                    $('#formAnalise').submit();
                }else{
                    alert('Município não finalizado.');
                }
            }
        });
    });

});
//-->
</script>
<form action="" id="formAnalise" method="post">
    <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubtituloCentro" colspan="2">Análise / Deliberação</td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="40%">
                Aplicou&nbsp; 25% da receita resultante de impostos na manuntenção e no desenvolvimento do ensino , relativos ao exercício anterior?
            </td>
            <td>
                <input type="radio" name="manutencao" value="S" /> Sim
                <input type="radio" name="manutencao" value="N" /> Não
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Preencheu completamente as informações requeridas pelo SIOPE?
            </td>
            <td>
                <input type="radio" name="siope" value="S" /> Sim
                <input type="radio" name="siope" value="N" /> Não
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Demonstrou o impacto financeiro provocado pela aplicação da&nbsp; Lei 11.738 nas respecticas finanças govetrnamentais?
            </td>
            <td>
                <input type="radio" name="financas" value="S" /> Sim
                <input type="radio" name="financas" value="N" /> Não
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Percentual de aplicação dos recursos vinculados à educação em remuneração do magistério - PARM
            </td>
            <td>
                <?php
                $parm = empty($_POST['parm']) ? '' : $_POST['parm'];
                echo campo_texto('parm', 'S', 'S', 'PARM', 10, 10, '', '', '', '', 0, 'id="parm" onkeyup="this.value=mascaraglobal(&quot;##,##%&quot;,this.value)"', '', $parm );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Percentual de aplicação dos recursos do FUNDEB na remineração do magistério - PAFR
            </td>
            <td>
                <?php
                $pafr = empty($_POST['pafr']) ? '' : $_POST['pafr'];
                echo campo_texto('pafr', 'S', 'S', 'PAFR', 10, 10, '', '', '', '', 0, 'id="pafr" onkeyup="this.value=mascaraglobal(&quot;##,##%&quot;,this.value)"', '', $pafr );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Percentual de aplicação das receitas de impostos e tranferencias despedas de MDE - PMDE
            </td>
            <td>
                <?php
                $pmde = empty($_POST['pmde']) ? '' : $_POST['pmde'];
                echo campo_texto('pmde', 'S', 'S', 'PMDE', 10, 10, '', '', '', '', 0, 'id="pmde" onkeyup="this.value=mascaraglobal(&quot;##,##%&quot;,this.value)"', '', $pmde );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                Impacto financeiro provocado pela aplicação da Lei 11.738/2008, nas finanças governamentais - IFLEI
            </td>
            <td>
                <?php
                $iflei = empty($_POST['iflei']) ? '' : $_POST['iflei'];
                echo campo_texto('iflei', 'S', 'S', 'IFLEI', 10, 10, '', '', '', '', 0, 'id="iflei" onkeyup="this.value=mascaraglobal(&quot;##,##%&quot;,this.value)"', '', $iflei );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">
                &nbsp;
            </td>
            <td>
                <input type="button" value="Analisar" class="analisar"/>
            </td>
        </tr>
    </table>
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>"/>
</form>