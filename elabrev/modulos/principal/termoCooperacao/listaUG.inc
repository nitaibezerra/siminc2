<?php

if ($_REQUEST['download'] == 'manual') {

    $caminho = APPRAIZ."www/manualTermoExecucaoDescentralizadoV2.pdf";
    if (!is_file($caminho)) {
        echo"<script>
                alert('Arquivo não encontrado na pasta.');
                window.location.href = 'elabrev.php?modulo=principal/termoCooperacao/listaUG&acao=A';
            </script>";
        exit();
    }

    echo "<script>window.location.href = '".$caminho."';</script>";
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(document).ready(function(){
		$('.listaTermos').click(function(){
			window.location = 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A&ungcod='+$(this).attr('id');
		});
	});
</script>

<?php

$_SESSION['elabrev']['ungcodlista'] = null;

#Inclui funções do Termo de Execução Descentralizado
require_once '_funcoes_termoCooperacao.php';

//Cabeçalho do sistema
include  APPRAIZ."includes/cabecalho.inc";
echo "</br>";

?>

<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td><b>Manual</b></td>
    </tr>
    <!--
    <tr>
        <td align="left" valign="top" width="90%">

            <table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <thead>
                    <tr bgcolor="#e9e9e9" align="left">
                        <td width="80px" align="left">
                            <?php $caminho = APPRAIZ."www/manualTermoExecucaoDescentralizadoV2.pdf";
                            if (is_file($caminho)) { ?>
                                <a target="_blank" style="cursor: pointer; color: blue;" href="/manualTermoExecucaoDescentralizadoV2.pdf"><b>
                                <img src="../imagens/pdf.gif" width="18px" border="0"> Manual do Termo de Execução Descentralizada</b></a>
                            <? } else { ?>
                                Arquivo inexistente;
                            <? } ?>
                        </td>
                    </tr>
                </thead>
            </table>
        </td>
    </tr>
    -->
</table>
<br>
<?	montaAbaTermoCooperacaoListaUGs();
monta_titulo(MODULO_NAME,$titulo_modulo);

$_SESSION['elabrev']['tcpid'] = null;
$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

//ver($perfis);

$qtdUngcods = 0;
if (!empty($_SESSION['elabrev']['ungcod'])) {
    if (is_array($_SESSION['elabrev']['ungcod'])) {
        $qtdUngcods = count($_SESSION['elabrev']['ungcod']);
    }
    else if ($_SESSION['elabrev']['ungcod'] > 0) {
        $qtdUngcods = 1;
    }
}

//ver($qtdUngcods);
#Se usuário é vinculado a apenas uma ung é diredionado diretamente a listagem de termos.
if ($qtdUngcods == 1 &&
    !in_array(PERFIL_SUPER_USUARIO, $perfis) &&
    !in_array(PERFIL_CGSO, $perfis) &&
// 		!in_array(PERFIL_DIRETORIA, $perfis) &&
    !in_array(PERFIL_SECRETARIO, $perfis) &&
    !in_array(PERFIL_SUBSECRETARIO, $perfis)
   )
{
    echo "
        <script>
            window.location = 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A&ungcod=".$_SESSION['elabrev']['ungcod']."';
        </script>
    ";
}

// Se não for super usuário seta as responsabilidades
if (!in_array(PERFIL_SUPER_USUARIO, $perfis)) {

    // Verifica se tem responsabilidade de Unidade Gestora
    if (in_array(PERFIL_PROREITOR_ADM, $perfis) ||
    in_array(PERFIL_REITOR, $perfis) ||
    in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis) ||
    in_array(PERFIL_SECRETARIA, $perfis) ||
    in_array(PERFIL_SECRETARIO, $perfis) ||
    in_array(UO_EQUIPE_TECNICA, $perfis)){
        if (isset($_SESSION['elabrev']['ungcod']) && !empty($_SESSION['elabrev']['ungcod'])) {
            $where[] = is_array($_SESSION['elabrev']['ungcod']) ?
                "ung.ungcod in ('".implode("','", $_SESSION['elabrev']['ungcod'])."')" :
                "ung.ungcod = '{$_SESSION['elabrev']['ungcod']}'";
        } else {
            $where[] = "ung.ungcod IS NULL";
        }
    }

    // Verifica se tem perfil de Concedente para CGSO e Gestor Orçamentario do Concedente, Unidade Gestora
    else if (in_array(PERFIL_SUBSECRETARIO, $perfis) || in_array(PERFIL_CGSO, $perfis)) {
        if (isset($_SESSION['elabrev']['ungcod']) && !empty($_SESSION['elabrev']['ungcod'])) {
            if(is_array($_SESSION['elabrev']['ungcod'])){
                if (in_array(UG_CGSO, $_SESSION['elabrev']['ungcod'])) {
                    $where[] = "ung.ungcod NOT IN ('153978','154003','153173')";
                }
            } else if($_SESSION['elabrev']['ungcod'] == UG_CGSO) {
                $where[] = "ung.ungcod NOT IN ('153978','154003','153173')";
            } else {
                $where[] = is_array($_SESSION['elabrev']['ungcod']) ?
                    "ung.ungcod IN ('".implode("','", $_SESSION['elabrev']['ungcod'])."')" :
                    "ung.ungcod = '{$_SESSION['elabrev']['ungcod']}'";
            }
        } else {
            $where[] = "ung.ungcod IS NULL";
        }
    }

    // Verifica se tem resposabilidade de Unidade
    else if (in_array(UO_CONSULTA_ORCAMENTO, $perfis) ||
             in_array(AUDITOR_INTERNO, $perfis) ||
             in_array(PERFIL_DIRETOR_ADMIM, $perfis))
    {
        if(isset($_SESSION['elabrev']['unicod']) && !empty($_SESSION['elabrev']['unicod'])){
            $where[] = is_array($_SESSION['elabrev']['unicod']) ?
                "uni.unicod in ('".implode("','", $_SESSION['elabrev']['unicod'])."')" :
                "uni.unicod = '{$_SESSION['elabrev']['unicod']}'";
        }else{
            $where[] = "uni.unicod IS NULL";
        }
    }

    // Verifica se tem resposabilidade de Diretoria
    else if (in_array(PERFIL_DIRETORIA, $perfis)) {
        if (isset($_SESSION['elabrev']['dircod']) && !empty($_SESSION['elabrev']['dircod'])) {
            $inner[] = "INNER JOIN elabrev.diretoria dir ON dir.ungcod = ung.ungcod";
            $where[] = is_array($_SESSION['elabrev']['dircod']) ?
                "dir.dircod in ('".implode("','", $_SESSION['elabrev']['dircod'])."')" :
                "dir.dircod = '{$_SESSION['elabrev']['dircod']}'";
        } else {
            $where[] = "ung.ungcod IS NULL";
        }
    }

    // Verifica se tem responsabilidade de Coordenação
    else if (in_array(PERFIL_COORDENADOR_SEC, $perfis)) {
        if (isset($_SESSION['elabrev']['cooid']) && !empty($_SESSION['elabrev']['cooid'])) {
            $inner[] = "INNER JOIN elabrev.coordenacao coo ON coo.ungcodconcedente = ung.ungcod";
            $where[] = is_array($_SESSION['elabrev']['cooid']) ?
                "coo.cooid in ('".implode("','", $_SESSION['elabrev']['cooid'])."')" :
                "coo.cooid = '{$_SESSION['elabrev']['cooid']}'";
        } else {
            $where[] = "ung.ungcod IS NULL";
        }
    }

    if (in_array(PERFIL_DIRETORIA_FNDE, $perfis) || in_array(PERFIL_AREA_TECNICA_FNDE, $perfis)) {
        $where[] = "ung.ungcod = '" . UG_FNDE . "'";
    }
}

$where[] = "ung.ungcod NOT IN ('150020')"; //retirando a EBSERH que estava listando em duplicidade

$sql = "
    SELECT DISTINCT
        '<a class=\"listaTermos\" id=\"'||ung.ungcod||'\" style=\"color:blue;cursor:pointer\">'||ung.ungcod||'</a>' as codigo,
        '(' || uni.unicod || ') - ' || ung.ungdsc as descricao
    FROM
        public.unidadegestora ung
    INNER JOIN unidade uni
            ON uni.unicod = ung.unicod
    ".(is_array($inner) ? " ".implode(" ", $inner) : '')."
    WHERE
        ungstatus = 'A'
    ".(is_array($where) ? " AND ".implode(" AND ", $where) : '')."
    ORDER BY
        2
";

//ver($sql, $_SESSION['elabrev'], d);
$cabecalho = Array("Numero da</br> Unidade Gestora","UO/Unidade Gestora");
$db->monta_lista($sql, $cabecalho, 10000, 10, 'N', '95%', 'N', '', '', '', '');
?>