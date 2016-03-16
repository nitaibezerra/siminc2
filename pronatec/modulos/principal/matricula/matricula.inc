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
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
include APPRAIZ . "includes/cabecalho.inc";

$redePerfil = 'MEC';

/* Retornando o Programa (Modalidade de Entrada) */
$sqlPrograma = <<<QUERY
SELECT DISTINCT
    co_tipo_programa as codigo,
    ds_tipo_programa as descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_tipo_programa
QUERY;


/* Retornando a Rede Ofertante */
$sqlOferntatne = <<<QUERY
SELECT DISTINCT
    co_rede_ofertante as codigo,
    ds_rede_ofertante as descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_rede_ofertante
QUERY;

/* Retornando a Situação da Matricula */
$sqlStatusMatricula = <<<QUERY
SELECT DISTINCT
    co_tipo_situacao_matricula as codigo,
    ds_tipo_situacao_matricula as descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_tipo_situacao_matricula
QUERY;

/* Retornando a Situação da Oferta */
$sqlEixoTecnologico = <<<QUERY
SELECT DISTINCT
    co_eixo_tecnologico as codigo,
    ds_eixo_tecnologico as descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_eixo_tecnologico
QUERY;

/* Retornando a Situação da Oferta */
$sqlTipoCursos = <<<QUERY
SELECT DISTINCT
    co_tipo_curso AS codigo,
    ds_tipo_curso AS descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_tipo_curso
QUERY;

global $db;
$dataDadosAtualizados = $db->pegaUm("SELECT to_char(dt_materializacao,'dd/mm/yyyy') FROM pronatec.matricularesumo LIMIT 1");
monta_titulo("Bolsa Formação", "Matrículas");
?>
<style>
    .linkTabela {
        color: #008000;
        cursor: pointer;
        float: left;
        font-size: 17px;
        font-weight: 900;
        width: 20px;
    }
    .bg-highligh-container {
        background-color: #dcdcdc;
        width: 100%;
        height: 25px;
        float: left;
        line-height: 25px;
    }
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
    function filtrarResultados(uf, municipio) {
        $('#filtros').hide();
        $('#voltarFiltros').show('fast');
        selectAllOptions(document.getElementById('rede_ofertante'));
        selectAllOptions(document.getElementById('status_matricula'));
        selectAllOptions(document.getElementById('eixo_tecnologico'));
        selectAllOptions(document.getElementById('programa'));
        selectAllOptions(document.getElementById('tipo_curso'));

        var filtroEstado = (uf != '') ? '&sg_uf=' + uf : '';
        var filtroMunicipio = (municipio != '') ? '&co_municipio=' + municipio : '';

        ajaxatualizar('requisicao=filtrar&tipoResultado=matricula&' + $('#frmPronatec').serialize() + filtroEstado + filtroMunicipio, 'divResultado');
        $('#divResultado').show();
    }
    function voltarFiltros() {
        $('#filtros').show();
        $('#voltarFiltros').hide('fast');
        $('#divResultado').hide('fast');
    }

    function ajaxatualizar(params, iddestinatario) {
        document.getElementById(iddestinatario).innerHTML = "<br/><br/><br/><center><b>Carregando...<b></center>";
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: params,
            async: false,
            success: function(html) {
                if (iddestinatario != '') {
                    document.getElementById(iddestinatario).innerHTML = html;
                }
            }
        });
    }
    function detalharMatriculaPorEstado(uf) {
        filtrarResultados(uf);
        $('#voltarMatriculaPorMunicipio').remove();
        voltar = '<div id="divVoltarEstado" class="linkTabela" title="Voltar" onclick="voltarMatriculaPorEstado()"> < </div>';
        $('.listagem').before(voltar);
    }
    function voltarMatriculaPorEstado() {
        filtrarResultados();
    }
    function detalharMatriculaPorMunicipio(uf, municipio) {
        filtrarResultados('', municipio);
        $('#divVoltarEstado').remove();
        voltar = '<div id="divVoltarMunicipio" class="linkTabela" title="Voltar" onclick="voltarMatriculaPorMunicipio(\'' + uf + '\')"> < </div>';
        $('.listagem').before(voltar);
    }
    function voltarMatriculaPorMunicipio(uf) {
        detalharMatriculaPorEstado(uf);
    }
</script>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td>
            <div style="float: left; "> <b>Acessando perfil: <? echo $redePerfil; ?> </b></div>
            <div style="float: right;"> Dados atualizados em: <b><? echo $dataDadosAtualizados; ?> </b></div>
            <div id="voltarFiltros" style="clear: both; display: none;"> <input type="button" class="botao" value="Modificar Filtros" onclick="voltarFiltros();"/> </div>

        </td>
    </tr>
</table>
<form id="frmPronatec" action="?modulo=inicio&acao=C&filtro=true" >
    <table id="filtros" align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
        <tr>
            <td colspan="10" class="subTituloCentro">
                Filtros
            </td>
        </tr>
        <tr>
            <td>
                <strong class="bg-highligh-container">Modalidade de Entrada:</strong><br />
                <? echo combo_popup('programa', $sqlPrograma, 'Modalidade de Entrada', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td>
                <strong class="bg-highligh-container">Rede Ofertante:</strong><br/>
                <? echo combo_popup('rede_ofertante', $sqlOferntatne, 'Rede Ofertante', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td>
                <strong class="bg-highligh-container">Situação da Matrícula:</strong><br/>
                <? echo combo_popup('status_matricula', $sqlStatusMatricula, 'Situação da Matricula', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td>
                <strong class="bg-highligh-container">Eixo Tecnológico:</strong><br/>
                <? echo combo_popup('eixo_tecnologico', $sqlEixoTecnologico, 'Eixo Tecnológico', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td>
                <strong class="bg-highligh-container">Tipo de Curso:</strong><br/>
                <? echo combo_popup('tipo_curso', $sqlTipoCursos, 'Tipo Curso', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
        </tr>
        <tr>
            <td colspan="10">
        <br/><center><input  type="button" style="width: 100px;" value="Filtrar" onclick="filtrarResultados();"/></center>
        </td>
        </tr>
    </table>
</form>
<br/>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td class="subTituloCentro">
            Dados para <?php echo $_SESSION['exercicio']; ?>
        </td>
    </tr>
    <tr>
        <td>
            <div id="divResultado" ></div>
        </td>
    </tr>
</table>