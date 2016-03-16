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

$redePerfil = 'MEC';
global $db;
$dataDadosAtualizados = $db->pegaUm("SELECT to_char(dt_materializacao,'dd/mm/yyyy') FROM pronatec.ofertaresumo LIMIT 1");
monta_titulo( "Bolsa Formação", "Rede Ofertante");

/* Retornando a Rede Ofertante */
$sqlOferntatne = <<<QUERY
SELECT DISTINCT
    co_rede_ofertante as codigo,
    ds_rede_ofertante as descricao
FROM
    pronatec.ofertaresumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_rede_ofertante
QUERY;

$redeOfertante = false;
/* Rede Ofertante */
if (count($_GET['rede_ofertante']) > 0 && $_GET['rede_ofertante'][0] <> '') {
   $redeOfertante = implode(',', $_GET['rede_ofertante']);
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<script>
    $( document ).ready(function() {
     $('#atualizar').click(function(){
       selectAllOptions(document.getElementById('rede_ofertante'));
       location.href = "pronatec.php?modulo=principal/redeofertante/redeofertante&acao=A&"+$('#frmPronatec').serialize();
    });
});
   
</script>
<br>
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
    #atualizar{
        cursor:pointer;
    }

</style>
<form id="frmPronatec" name="frmPronatec" action="pronatec/pronatec.php?modulo=principal/redeofertante/redeofertante&acao=A" >
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td>
            <div style="float: left; "> <b>Acessando perfil: </b>
            <? echo combo_popup('rede_ofertante', $sqlOferntatne, 'Rede Ofertante', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            <img id="atualizar" src="../imagens/refresh.gif" title="Atualizar Infomrações"/>
            </div>
            <div style="float: right;"> Dados atualizados em: <b><? echo $dataDadosAtualizados; ?> </b>
                <br/><a href=""> [Baixar detalhes de Ofertas de Turma] </a>
                <br/><a href=""> [Baixar detalhes de Matrículas] </a>
            </div>
        </td>
    </tr>
</table>
</form>
<table id="filtros" align="center" width="1000px" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td valign="center">
            <div id="divMatricula" class="divGraf">
                 <?
                 if($redeOfertante){
                     $where=" AND co_rede_ofertante IN ({$redeOfertante}) ";
                 }
                $sql = " SELECT DISTINCT
                            ds_tipo_situacao_matricula as descricao,
                            sum(nu_alunos) as valor
                        FROM
                            pronatec.matricularesumo
                        WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
                        {$where}
                        GROUP BY
                            ds_tipo_situacao_matricula ";
                $dados = $db->carregar($sql);
                #var_dump($dados);die;
                echo geraGrafico($dados, "matriculas", "Matrículas {$_SESSION['exercicio']}", "", "", "", false, "", "", "", true);
                ?>
                <a href="pronatec.php?modulo=principal/matricula/matricula&acao=A" class="linkMais">+ Mais</a>
            </div>
            <div id="divOfertaTurma" class="divGraf">
                <?
                $sql = "     SELECT DISTINCT
                            ds_status_oferta_turma as descricao,
                            sum(nu_vagas) as valor
                        FROM
                            pronatec.ofertaresumo
                        WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
                         {$where}
                        GROUP BY
                            ds_status_oferta_turma ";
                $dados = $db->carregar($sql);
                #var_dump($dados);die;
                echo geraGrafico($dados, "oferta_turma", "Ofertas de Turma {$_SESSION['exercicio']}", "", "", "", false, "", "", "", true);
                ?>
                <a href="pronatec.php?modulo=principal/ofertaturma/ofertaturma&acao=A" class="linkMais">+ Mais</a>
            </div>
        </td>
    </tr>
</table>
