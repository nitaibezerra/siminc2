<?
/*
  Sistema Simec
  Setor responsvel: MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
  Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
  Mdulo:inicio.inc
  Finalidade: permitir abrir a página inicial
 */
//Chamada de programa
include_once '_funcoes.php';

/* Funções de Gráficos do Cockpit */
include_once '../../pde/www/_funcoes_cockpit.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
include APPRAIZ . "includes/cabecalho.inc";

global $db;
$dataDadosAtualizados = $db->pegaUm("SELECT to_char(dt_materializacao,'dd/mm/yyyy') FROM pronatec.ofertaresumo LIMIT 1");
monta_titulo( "Bolsa Formação", "");
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<script>
</script>
<br>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td>
            <div style="float: right;"> Dados atualizados em: <b><? echo $dataDadosAtualizados; ?> </b></div>
        </td>
    </tr>
</table>
<style>
    .divGraf{
        width: 450px;
        height: 430px;
        float: left;
        margin-top: 20px;
        margin-left: 20px;
        background-color: yellowgreen;
        -webkit-border-radius: 20px;
        -moz-border-radius: 20px;
        border-radius: 20px;
    }

    .linkMais{
        float: right;
        margin-right: 10px;
        color: #FFF;
        font-size: 14px;
    } 

</style>
<table id="filtros" align="center" width="1000px" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td valign="center">
            <div id="divMatricula" class="divGraf">
                 <?
                $sql = " SELECT DISTINCT
                            ds_tipo_programa as descricao,
                            sum(nu_alunos) as valor
                        FROM
                            pronatec.matricularesumo
                        WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
                        GROUP BY
                            ds_tipo_programa ";
                $dados = $db->carregar($sql);
                #var_dump($dados);die;
                echo geraGrafico($dados, "matriculas", "Matrículas {$_SESSION['exercicio']}", "", "", "", true, "", "", "", true);
                #echo geraGrafico($dados, "matriculas", "Matrículas {$_SESSION['exercicio']}","","{point.percentage:.2f} %","",false,"","","",true);
                ?>
                <a href="pronatec.php?modulo=principal/matricula/matricula&acao=A" class="linkMais">+ Mais</a>
            </div>
            <div id="divOfertaTurma" class="divGraf">
                <?
                $sql = " SELECT DISTINCT
                            ds_rede_ofertante as descricao,
                            sum(nu_vagas) as valor
                        FROM
                            pronatec.ofertaresumo
                        WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
                        GROUP BY
                            ds_rede_ofertante ";
                $dados = $db->carregar($sql);
                #var_dump($dados);die;
                echo geraGrafico($dados, "oferta_turma", "Ofertas de Turma {$_SESSION['exercicio']}", "", "", "", true, "", "", "", true);
                ?>
                <a href="pronatec.php?modulo=principal/ofertaturma/ofertaturma&acao=A" class="linkMais">+ Mais</a>
            </div>
        </td>
    </tr>
</table>
