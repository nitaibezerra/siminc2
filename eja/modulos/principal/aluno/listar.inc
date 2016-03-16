<?php
$dataAtual = date('Y/m/d');
$dataLimite = date('2015/01/19');

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $sql = "SELECT
                a.atecpf,
                a.atenomedoaluno,
                a.atesexo,
                to_char(a.atedatanasc, 'DD/MM/YYYY'),
                a.atenomedamae
            FROM
                eja.alunoturmaeja a
            WHERE
                a.nteid = {$_REQUEST['nteid']}
            ORDER BY
                a.ateid ";
    return $sql;
}

if ($_REQUEST['tiporelatorio']) {

    $sql = monta_sql();

//    ver($sql);

    $cabecalho = array(
        "CPF",
        "Nome",
        "Sexo",
        "Data de nascimento",
        "Nome da Mãe");


    echo '<html>
            <head>
                <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('95') . ' 
                    </center>
        ';

    //recuperando entidade e uf, caso usuário logado não tenha acesso a tela de início e código já carregue a entidade.
    $entid = !empty($_SESSION['entid']) ? $_SESSION['entid'] : $_SESSION['escolaUsuarioLogado'];
    $uf = !empty($_SESSION['uf']) ? $_SESSION['uf'] : $_SESSION['ufEscolaUsuarioLogado'];

    if ($entid && $uf) {
        //recuperando dados UF, Município, Nome da Escola, Número da Turma e Status da Turma #demanda 274298
        $dadosTurma = $db->carregar("SELECT DISTINCT
                                    ent.entnome as escola,
                                    est.estuf || ' - ' || est.estdescricao as uf,
                                    mun.mundescricao as municipio,
                                    'Turma ' || ntesequencial || '' AS turma,
                                    CASE 
                                        WHEN nov.nteenviofnde = 'V' THEN 'Validada'
                                        WHEN nov.ntefechaturma = 'A' THEN 'Aberta'
                                        ELSE 'Fechada'
                                    END AS status
                                FROM entidade.entidade ent
                                LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
                                INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                                INNER JOIN territorios.estado est ON est.estuf = mun.estuf
                                INNER JOIN eja.novaturmaeja nov ON nov.pk_cod_entidade = ent.entid
                                WHERE
                                    est.estuf = '" . $uf . "'
                                AND 
                                    ent.entid = " . $entid . "
                                AND 
                                    nov.nteid = " . $_REQUEST['nteid']);
        is_array($dadosTurma) ? $dadosTurma : array();
        foreach ($dadosTurma as $turma) {
            echo '<table width="95%" align="center">
                        <tr>
                          <td><b>UF: </b></td>
                          <td><font color="#777777">' . $turma['uf'] . '</font></td>
                          <td><b>Município: </b></td>
                          <td><font color="#777777">' . $turma['municipio'] . '</font></td>
                        </tr>

                        <tr>
                          <td><b>Nome da Escola:</b></td>
                          <td><font color="#777777">' . $turma['escola'] . '</font></td>

                          <td><b>Situação da Turma:</b></td>
                          <td><font color="#777777">' . $turma['status'] . '</font></td>

                          <td><b>Número da Turma:</b></td>
                          <td><font color="#777777">' . $turma['turma'] . '</font></td>
                        </tr>
                  </table> ';
        }
    } else {
        echo "<tr><td colspan=10 align=center><font color=red>Não existem Escolas Associadas a este Perfil.</font></td></tr>";
    }

    $db->monta_lista($sql, $cabecalho, 30, 50, 'N', 'left', 'N', 'N');
    echo '<table><tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>';
    echo '<td><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
    echo '</body>';
    die();
}
require (APPRAIZ . 'includes/library/simec/Listagem.php');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$turma = buscarDadosTurma($_REQUEST['nteid']);
$listaPerfil = pegaPerfilGeral();
if ($_REQUEST['requisicao'] == 'excluir') {

    if (validarTurmaFechada($_REQUEST['nteid'], 'Excluir') === true) {

        $sql = "DELETE FROM eja.alunoturmaeja WHERE ateid = {$_REQUEST['ateid']}";

        if ($db->executar($sql)) {
            $db->commit();
        }
        $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=principal/aluno/listar&acao=C&nteid={$turma['nteid']}'; </script>";
        echo $scriptRedirecionar;
        die;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-lg-12">

    <div class="well">
        <h4 id="forms">Turma <?php echo $turma['ntesequencial']; ?> - Lista de Alunos</h4>
        <?php echo mostrarDadosEscola($turma['pk_cod_entidade']); ?>

    </div>



    <!--<br />-->

    <?php
    if (
            in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
            in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)
    ) {
        if (strtotime($dataAtual) < strtotime($dataLimite) || $_SESSION['TipoUsuario']) {
            $acoesLista = "a.ateid,";
        } else {
            $textoLimite = "<center><h4><b>*Data limite (18/01/2015) para alteração e exclusão de CPF atingida.</b></h4></center><br>";
        }
    }

    $sql = "
        SELECT
            $acoesLista
            a.atecpf,
            a.atenomedoaluno,
            case when a.atesexo = 'F' then 'Feminino' when a.atesexo = 'M' then 'Masculino' else '' end as atesexo,
            to_char(a.atedatanasc, 'DD/MM/YYYY'),
            a.atenomedamae
        FROM
            eja.alunoturmaeja a
        WHERE
            a.nteid = {$_REQUEST['nteid']}
        ORDER BY
            a.ateid";

    $cabecalho = array(
        "CPF",
        "Nome",
        "Sexo",
        "Data de Nascimento",
        "Nome da Mãe");

    $listagem = new Simec_Listagem();
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);

    if (
            in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
            in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)
    ) {

        if (strtotime($dataAtual) < strtotime($dataLimite) || $_SESSION['TipoUsuario']) {
            $listagem->setAcoes(array(
                'edit' => 'editLimite',
                'delete' => 'deleteLimite'
            ));
        }
    }
    echo $textoLimite;
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>

    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-primary" id="btnVoltar" type="button" >Voltar</button>
            <? if( in_array(PERFIL_EJA_GESTOR_MEC,$listaPerfil) || 
            in_array(PERFIL_EJA_GESTOR,$listaPerfil) || 
            in_array(PERFIL_EJA_GESTOR_ESTADUAL,$listaPerfil) || 
            in_array(PERFIL_EJA_GESTOR_MUNICIPAL,$listaPerfil) || 
            in_array(PERFIL_EJA_DIRETOR_ESCOLA,$listaPerfil) ||
            in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil)){ ?>
            <button class="btn btn-info" id="btnRelatorio" type="button"><i class="glyphicon glyphicon-th-list"></i> Relatório </button>
            <?}?>
<?php
if ((in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
        in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)) &&
        $turma['ntefechaturma'] != 'F' && ((strtotime($dataAtual) < strtotime($dataLimite)) || $_SESSION['TipoUsuario'])
):
    ?>
                <button class="btn btn-success" id="btnCadastrar" type="button">
                    <i class="glyphicon glyphicon-upload"></i>
                    Cadastrar novo Aluno
                </button>
<?php endif; ?>
        </div>
    </div>
</div>
<script>
    jQuery(document).ready(function() {
        jQuery('#btnVoltar').click(function() {
            voltar();
        });

        jQuery('#btnCadastrar').click(function() {
            cadastrar();
        });

        jQuery('#btnRelatorio').click(function() {
            gerarRelatorio('html');
        });

    });

    function voltar() {
        window.location = 'eja.php?modulo=inicio&acao=C';
    }

    function editLimite(ateid) {
        window.location = "eja.php?modulo=principal/aluno/cadastrar&acao=C&acao=C&nteid=<?php echo $turma['nteid']; ?>&ateid=" + ateid;
    }

    function deleteLimite(ateid) {
        if (confirm("Deseja realmente excluir esse aluno?")) {
            window.location += '&requisicao=excluir&ateid=' + ateid;
        }
    }

    function cadastrar() {
        window.location = "eja.php?modulo=principal/aluno/cadastrar&acao=C&acao=C&nteid=<?php echo $turma['nteid']; ?>";
    }

    //gerando relatório via pop, submetendo a mesma tela.
    function gerarRelatorio(tipo) {
        var url = "eja.php?modulo=principal/aluno/listar&acao=C";
        var params = "&nteid=<?php echo $turma['nteid']; ?>&tiporelatorio=" + tipo;
        window.open(url + params, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }


</script>