<?php
$dataAtual = date('Y/m/d');
$dataLimite = date('2015/04/06');
$cpf = $_SESSION['usucpf'];
$listaPerfil = pegaPerfilGeral();
$filtroAno = " and nteexercicio = '2015'";

$_SESSION['TipoUsuario'] = verificaUsuEsp($cpf);

if (!$_SESSION['TipoUsuario']) {
    if (strtotime($dataAtual) >= strtotime($dataLimite)) {
        $botao = "disabled = 'disabled'";
        $textoLimite = "<b>* Data limite (18/01/2015) para criação de nova turma atingida.</b>";
        if ($_SESSION['EJA']['mostraAviso'] == false) {
//ver($_SESSION['EJA']['mostraAviso'],d);
//alert('Aviso! Informamos que a partir de 19/01/2015 a abertura de turmas e cadastro de alunos está temporariamente suspenso. As turmas abertas com alunos cadastrados terão até o dia 31 de janeiro para serem homologadas.');
            //alert('A partir de hoje (06/04/2015) é possível confirmar a continuidade dos novos alunos de EJA a fim de possibilitar o pagamento da segunda parcela. Oriente-se pelo manual disponível no canto superior direito da página. Informamos também que o Sistema continua suspenso para o cadastro de novas matrículas de EJA para 2015.');
            echo "<script>
  alert('IMPORTANTE! SEGUNDA PARCELA - NOVAS TURMAS DE EJA - Informamos que a confirmação dos alunos para recebimento da segunda parcela só será possível até o dia 30 de setembro de 2015. Obs.: Para realizar o procedimento consulte o Manual no canto superior direito da tela.');
</script>";
            $_SESSION['EJA']['mostraAviso'] = true;
        }
    }
}

require (APPRAIZ . 'includes/library/simec/Listagem.php');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";


$listaEscola = buscarEscolaUsuarioLogado($cpf, $listaPerfil);


if ($_REQUEST['requisicao'] == 'selecionarEscola') {
    $entid = $_REQUEST['entid'];
    $_SESSION['entid_selecionada'] = $entid;
    $scriptRedirecionar = "
            <script>
//                alert('Escola selecionada com sucesso!');
                window.location = 'eja.php?modulo=inicio&acao=C';
            </script>
        ";
    echo $scriptRedirecionar;
    die;
}

# Verifica se o usuário tem várias escolas
if (count($listaEscola) > 1) {
    $entid = $_SESSION['entid_selecionada'];
    # Verifica se ele já selecionou a escola para gerenciar turma
    if (empty($entid)) {
        # Exibi a tela para selecionar escola
        include APPRAIZ . "eja/modulos/principal/escola/selecionar-escola.inc";
        die;
    } else {
        # Carrega informações da escola selecionada
        $escolaUsuarioLogado = buscarDadosEscola($entid);
    }
} else {
    $escolaUsuarioLogado = is_array($listaEscola) ? current($listaEscola) : NULL;
}
$estadoUsuarioLogado = buscarEstadoUsuarioLogado($cpf);
$municipioUsuarioLogado = buscarMunicipioUsuarioLogado($cpf);
//ver($listaPerfil);
if (!empty($_REQUEST['uf']))
    $_SESSION['uf'] = $_REQUEST['uf'];
if (!empty($_REQUEST['muncod']))
    $_SESSION['muncod'] = $_REQUEST['muncod'];
if (!empty($_REQUEST['entid']))
    $_SESSION['entid'] = $_REQUEST['entid'];

if ($_REQUEST['requisicao'] == 'fechar') {
    # O sistema só permite fechar turma se existir pelo menos um aluno
    $qtdAlunosAtivos = buscarQuantidadeAluno($_REQUEST['nteid']);
    if (empty($qtdAlunosAtivos)) {
        $scriptRedirecionar = "
                <script>
                    alert('Não foi possível fechar a turma porque ela não possui nenhum aluno cadastrado.');
                    window.location = 'eja.php?modulo=inicio&acao=C';
                </script>
            ";
        echo $scriptRedirecionar;
        die;
    }

    $sql = "
            UPDATE
                eja.novaturmaeja
            SET 
                ntefechaturma = 'F',
                ntedatafechaturma = NOW()
            WHERE
                nteid = {$_REQUEST['nteid']}";

    if ($db->executar($sql)) {
        $db->commit();
    }
    $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=inicio&acao=C'; </script>";
    echo $scriptRedirecionar;
    die;
}

if ($_REQUEST['requisicao'] == 'validar') {
    # O sistema só permite fechar turma se existir pelo menos um aluno
    $turma = buscarDadosTurma($_REQUEST['nteid']);
    if ($turma['ntefechaturma'] != 'F') {
        $scriptRedirecionar = "
                <script>
                    alert('Não foi possível validar a turma porque ela não está fechada.');
                    window.location = 'eja.php?modulo=inicio&acao=C';
                </script>
            ";
        echo $scriptRedirecionar;
        die;
    }

    $sql = "
            UPDATE
                eja.novaturmaeja
            SET
                nteenviofnde = 'V'
            WHERE
                nteid = {$_REQUEST['nteid']}";

    if ($db->executar($sql)) {
        $db->commit();
    }
    $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=inicio&acao=C'; </script>";
    echo $scriptRedirecionar;
    die;
}

if ($_REQUEST['requisicao'] == 'naovalidar') {

    $sql = "
            UPDATE
                eja.novaturmaeja
            SET
                nteenviofnde = 'N'
            WHERE
                nteid = {$_REQUEST['nteid']}";

    if ($db->executar($sql)) {
        $db->commit();
    }
    $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=inicio&acao=C'; </script>";
    echo $scriptRedirecionar;
    die;
}

if ($_REQUEST['requisicao'] == 'abrir') {
    # O sistema só permite abrir a turma caso as informações ja não tenham sido enviadas para o FNDE
    $turma = buscarDadosTurma($_REQUEST['nteid']);
    if (!empty($turma['ntedataenviofnde'])) {
        $scriptRedirecionar = "
                <script>
                    alert('Não foi possível abrir a turma porque as informações da turma já foram enviadas para o FNDE.');
                    window.location = 'eja.php?modulo=inicio&acao=C';
                </script>
            ";
        echo $scriptRedirecionar;
        die;
    }

    $sql = "
            UPDATE
                eja.novaturmaeja
            SET 
                ntefechaturma = 'A',
                ntedatafechaturma = NULL
            WHERE
                nteid = {$_REQUEST['nteid']}";

    if ($db->executar($sql)) {
        $db->commit();
    }
    $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=inicio&acao=C'; </script>";
    echo $scriptRedirecionar;
    die;
}

if ($_REQUEST['requisicao'] == 'excluir') {
    # O sistema só permite excluir a turma que não tenha nenhum aluno
    $qtdAlunosAtivos = buscarQuantidadeAluno($_REQUEST['nteid']);
    if (!empty($qtdAlunosAtivos)) {
        $scriptRedirecionar = "
                <script>
                    alert('Não foi possível apagar essa turma porque ela possui alunos cadastrados, por favor, apague todos os alunos para poder apagar a turma corretamente.');
                    window.location = 'eja.php?modulo=inicio&acao=C';
                </script>
            ";
        echo $scriptRedirecionar;
        die;
    }

    $sql = "
            UPDATE 
                eja.novaturmaeja
            SET 
                ntestatus = 'I'
            WHERE
                nteid = {$_REQUEST['nteid']}";

    if ($db->executar($sql)) {
        $db->commit();
    }
    $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=inicio&acao=C'; </script>";
    echo $scriptRedirecionar;
    die;
}

if ($_REQUEST['requisicao'] == 'salvar') {
    $numero = buscarSequencialUltimaTurma($_REQUEST['entid']);
    $ntesequencial = empty($numero) ? 1 : ++$numero;
    $ntedatainiturma = formata_data_sql($_REQUEST['ntedatainiturma']);

    $sqlAno = "select valexercicio from eja.valoraluno  where valstatus = 'A'";
    $anoAtual = $db->pegaUm($sqlAno);


    $sql = "
            INSERT INTO
                eja.novaturmaeja
            (
                pk_cod_entidade,
                ntesequencial,
                nivid,
                ntedatainiturma,
                usucpf,
                nteexercicio
            )
            VALUES (
                {$_REQUEST['entid']},
                {$ntesequencial},
                {$_REQUEST['nivid']},
                '{$ntedatainiturma}',
                '{$_SESSION['usucpf']}',
                '{$anoAtual}'    
            )";

    if ($db->executar($sql)) {
        $db->commit();
    }
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>

<div class="col-lg-12">
    <div align="right">
        <table class="listagem" border=1  align="right" cellspacing="3" cellpadding="3">
            <tr>
                <td align="center">
                    <b>Manuais para Download<b>
                            </td>
                            </tr>
                            <tr><td>Primeira Parcela</td></tr>
                            <tr>

                                <td align="left">
                                    <?
                                    echo '<a href="/eja/arquivos/manual_diretor.pdf" target="_blank" title="Clique para fazer o download">- MANUAL DIRETOR</a>';
                                    echo '<br><a href="/eja/arquivos/manual_gestor_estadual.pdf" target="_blank" title="Clique para fazer o download">- MANUAL GESTOR ESTADUAL</a>';
                                    echo '<br><a href="/eja/arquivos/manual_gestor_municipal.pdf" target="_blank" title="Clique para fazer o download">- MANUAL GESTOR MUNICIPAL</a>';
                                    echo '<br><a href="/eja/arquivos/manual_liberar_diretor.pdf" target="_blank" title="Clique para fazer o download">- MANUAL P/ LIBERAR DIRETOR ESCOLAR</a>';
                                    ?>
                                </td>
                            </tr>
                            <tr><td>Segunda Parcela</td></tr>
                            <tr>

                                <td align="left">
                                    <?php
                                    echo '<a href="/eja/arquivos/manual_segundaP_diretor.pdf" target="_blank" title="Clique para fazer o download">- MANUAL DIRETOR</a>';
                                    echo '<br><a href="/eja/arquivos/manual_segundaP_municipal.pdf" target="_blank" title="Clique para fazer o download">- MANUAL GESTOR MUNICIPAL</a>';
                                    ?>
                                </td>
                            </tr>
                            </table>
                            </div>

                            <br /><br /><BR><BR> <br /><br /><BR>

                            <div class="page-header">
                                <h4 id="forms">Lista de Turmas</h4>
                            </div>

                            <br />

                            <div class="well">
                                <?php
                                $_SESSION['escolaUsuarioLogado'] = $escolaUsuarioLogado['entid'];
                                $_SESSION['ufEscolaUsuarioLogado'] = $escolaUsuarioLogado['estuf'];

                                echo mostrarDadosEscola($escolaUsuarioLogado['entid']);
                                ?>
                                <fieldset class="text-center">
                                    <form id="formTurma" name="formTurma" method="POST" class="form-horizontal">
                                        <input type="hidden" name="requisicao" value="filtrar" />
                                        <?php
                                        if (
                                                in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
                                                in_array(PERFIL_EJA_CONSULTA, $listaPerfil) ||
                                                in_array(PERFIL_EJA_GESTOR, $listaPerfil) ||
                                                in_array(PERFIL_EJA_GESTOR_ESTADUAL, $listaPerfil) ||
                                                in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $listaPerfil)
                                        ):
                                            ?>

                                            <?php
                                            if (in_array(PERFIL_EJA_GESTOR_ESTADUAL, $listaPerfil)):
                                                $uf = $estadoUsuarioLogado['estuf'];
                                                ?>
                                                <input type="hidden" id="uf" name="uf" value="<?php echo $estadoUsuarioLogado['estuf']; ?>" />
                                                <div class="form-group">
                                                    <label for="file" class="col-lg-2 control-label">
                                                        UF:
                                                    </label>
                                                    <div class="col-lg-10">
                                                        <?php echo $estadoUsuarioLogado['estdescricao']; ?>
                                                    </div>
                                                </div>
                                            <?php elseif (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $listaPerfil)): ?>
                                                <input type="hidden" id="uf" name="uf" value="<?php echo $municipioUsuarioLogado['estuf']; ?>" />
                                                <div class="form-group">
                                                    <label for="file" class="col-lg-2 control-label">
                                                        UF:
                                                    </label>
                                                    <div class="col-lg-10">
                                                        <?php echo $municipioUsuarioLogado['estdescricao']; ?>
                                                    </div>
                                                </div>
                                            <?php else: ?>
                                                <div class="form-group">
                                                    <label for="file" class="col-lg-2 control-label">
                                                        UF:
                                                    </label>
                                                    <div class="col-lg-10">
                                                        <?php
                                                        $uf = @$_SESSION['uf'];
                                                        $sql = "
                                        SELECT DISTINCT
                                            e.estuf AS codigo,
                                            e.estuf || ' - ' || e.estdescricao AS descricao
                                        FROM
                                            territorios.estado e
                                        ORDER BY
                                            codigo ASC
                                    ";
                                                        inputCombo("uf", $sql, NULL, 'uf');
                                                        ?>
                                                    </div>
                                                </div>
                                            <?php endif; ?>

                                            <?php
                                            if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $listaPerfil)):
                                                $muncod = $municipioUsuarioLogado['muncod'];
                                                ?>
                                                <input type="hidden" id="muncod" name="muncod" value="<?php echo $municipioUsuarioLogado['muncod']; ?>" />
                                                <div class="form-group">
                                                    <label for="file" class="col-lg-2 control-label">
                                                        Município:
                                                    </label>
                                                    <div class="col-lg-10">
                                                        <?php echo $municipioUsuarioLogado['mundescricao']; ?>
                                                    </div>
                                                </div>
                                            <?php else: ?>
                                                <?php if (!empty($uf)): ?>
                                                    <div class="form-group">
                                                        <label for="file" class="col-lg-2 control-label">
                                                            Município:
                                                        </label>
                                                        <div class="col-lg-10">
                                                            <?php
                                                            $muncod = @$_SESSION['muncod'];
                                                            $sql = "
                                            SELECT
                                                m.muncod AS codigo,
                                                m.estuf || ' - ' || m.mundescricao AS descricao
                                            FROM
                                                territorios.municipio m
                                            WHERE
                                                m.estuf = '{$uf}'
                                            ORDER BY
                                                descricao
                                        ";
                                                            inputCombo("muncod", $sql, NULL, 'muncod');
                                                            ?>
                                                        </div>
                                                    </div>
                                                <?php endif; ?>
                                            <?php endif; ?>



                                            <?php if (!empty($muncod)): ?>
                                                <div class="form-group">
                                                    <label for="file" class="col-lg-2 control-label">
                                                        Escola:
                                                    </label>
                                                    <div class="col-lg-10">
                                                        <?php
                                                        $entid = @$_SESSION['entid'];
                                                        $sql = "
                                        SELECT DISTINCT
                                            ent.entid AS codigo,
                                            ent.entcodent || ' - ' || ent.entnome AS descricao,
                                            ent.entnome
                                        FROM entidade.entidade ent
                                            LEFT JOIN entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
                                            LEFT JOIN entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
                                            LEFT JOIN entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid
                                            LEFT JOIN entidade.entidade professor on professor.entid = feProfessor.entid
                                            LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
                                            LEFT JOIN entidade.entidadedetalhe entd ON entd.entid = ent.entid
                                            INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                                            INNER JOIN territorios.estado est ON est.estuf = mun.estuf
                                        WHERE
                                            ent.entstatus='A'
                                            AND feEscola.funid in (3,4)
                                            AND mun.muncod = '$muncod'
                                        ORDER BY
                                            ent.entnome ASC
                                    ";
                                                        inputCombo("entid", $sql, NULL, 'entid');
                                                        ?>
                                                    </div>
                                                </div>
                                            <?php endif; ?>
                                        <?php else: ?>
                                            <input type="hidden" id="entid" name="entid" value="<?php echo $escolaUsuarioLogado['entid']; ?>" />
                                            <input type="hidden" id="nivid" name="nivid" value="" />
                                            <input type="hidden" id="ntedatainiturma" name="ntedatainiturma" value="" />

                                            <button class="btn btn-success" id="btnInserir" type="button" <?php echo $botao; ?> title="Data limite 15/01/2015">
                                                <i class="glyphicon glyphicon-upload"></i>
                                                Criar nova Turma
                                            </button>
                                            <?php echo $textoLimite; ?>
                                        <?php endif; ?>
                                    </form>
                                </fieldset>
                            </div>

                            <br />

                            <?php
                            $dataLimiteVal = date('2015/01/30');
                            if (!$_SESSION['TipoUsuario']) {
                                if (strtotime($dataAtual) >= strtotime($dataLimiteVal)) {
                                    $textoVal = '*<b>A partir de hoje (31/01/2015) as turmas não podem ser homologadas.</b>';
                                }
                            }
                            $filtroEscola = $escolaUsuarioLogado ? " AND t.pk_cod_entidade = {$escolaUsuarioLogado['entid']} " : NULL;
                            if (!empty($_SESSION['entid']))
                                $filtroEscola = " AND t.pk_cod_entidade = {$_SESSION['entid']} ";

                            if (empty($filtroEscola))
                                $filtroEscola = " AND FALSE ";

                            $acoesLista = '';
                            $campoValidar = '';
                            if (
                                    in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil)
                            ) {
                                $cabecalho = array(
                                    "",
                                    "Turma",
                                    "Nivel",
                                    "Situação",
                                    "Validar Turma");
                                $acoesLista = 't.nteid,';
                                if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {
                                    $cadeado = "
                CASE WHEN t.ntefechaturma = 'A' THEN
                    '<img src=\"/imagens/cadeadoAberto.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"fechar(' || t.nteid || ')\" >'
                ELSE
                    '<img src=\"/imagens/cadiado.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"abrir(' || t.nteid || ')\" >'
                END AS ico_cadeado,
            ";
                                } else {
                                    $cadeado = "
                CASE WHEN t.ntefechaturma = 'A' THEN
                    '<img src=\"/imagens/cadeadoAberto.png\" width=\"18px\" \" >'
                ELSE
                    '<img src=\"/imagens/cadiado.png\"  width=\"18px\" \" >'
                END AS ico_cadeado,
            ";
                                }

                                if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {

                                    $campoValidar = " ,CASE WHEN t.nteenviofnde = 'N' THEN
				                    '<a href=\"javascript:validar(' || t.nteid || ');\"><span class=\"glyphicon glyphicon-thumbs-down\"></span></a>'
				               WHEN t.nteenviofnde = 'V' THEN
				                    '<a href=\"javascript:naoValidar(' || t.nteid || ');\"><span class=\"glyphicon glyphicon-thumbs-up\"></span></a>'
                                               WHEN t.nteenviofnde = 'E' THEN
                                               '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
				               END AS validar
            ";
                                } else {
                                    $campoValidar = " ,CASE WHEN t.nteenviofnde = 'N' THEN
				                    '<span class=\"glyphicon glyphicon-thumbs-down\"></span>'
				               WHEN t.nteenviofnde = 'V' THEN
				                    '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
                                               WHEN t.nteenviofnde = 'E' THEN
                                               '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
				               END AS validar
            ";
                                }
                            } else if (
                                    in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)
                            ) {
                                if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {
                                $cabecalho = array("", "Turma", "Nivel", "Situação");

                                $acoesLista = 't.nteid,';
                                }else{
                                $cabecalho = array("Turma", "Nivel", "Situação");

                                }
                                if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {
                                    $cadeado = "
                CASE WHEN nteenviofnde = 'V'
                    THEN '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'
                    ELSE
                        CASE WHEN ( t.ntefechaturma = 'A' )
                            THEN '<img src=\"/imagens/cadeadoAberto.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"fechar(' || t.nteid || ')\" >'
                            ELSE '<img src=\"/imagens/cadiado.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"abrir(' || t.nteid || ')\" >'
                        END
                END AS ico_cadeado,
            ";
                                } else {
                                    $cadeado = "
                CASE WHEN nteenviofnde = 'V'
                    THEN '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'
                    ELSE
                        CASE WHEN ( t.ntefechaturma = 'A' )
                            THEN '<img src=\"/imagens/cadeadoAberto.png\" width=\"18px\"\" >'
                            ELSE '<img src=\"/imagens/cadiado.png\" width=\"18px\" \" >'
                        END
                END AS ico_cadeado,
            ";
                                }
                            } else if (
                                    in_array(PERFIL_EJA_GESTOR_ESTADUAL, $listaPerfil) ||
                                    in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $listaPerfil)
                            ) {
                                $cabecalho = array("", "Turma", "Nivel", "Situação", "Validar Turma");

                                $cadeado = "
                CASE WHEN nteenviofnde = 'V'
                    THEN '<img src=\"/imagens/cadiado_p.png\" width=\"18px\" itle=\"Validada\">'
                    WHEN nteenviofnde = 'E'
                    --THEN '<img src=\"/imagens/cadeadoAberto.png\" style=\"cursor: pointer;\" width=\"18px\" >'
                   THEN '<img src=\"/imagens/cadiado.png\" title=\"Enviada para o FNDE\"style=\"cursor: pointer;\" width=\"18px\">'
                       -- END
                END AS ico_cadeado,
            ";

                                if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {
                                    $campoValidar = " 
                ,CASE WHEN t.nteenviofnde = 'N' 
                    THEN '<a href=\"javascript:validar(' || t.nteid || ');\"><span class=\"glyphicon glyphicon-thumbs-down\"></span></a>'
                    WHEN t.nteenviofnde = 'V' 
                    THEN '<a href=\"javascript:naoValidar(' || t.nteid || ');\"><span class=\"glyphicon glyphicon-thumbs-up\"></span></a>'
                     WHEN t.nteenviofnde = 'E' 
                    THEN '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
                END AS validar
            ";
                                } else {
                                    $campoValidar = " 
                ,CASE WHEN t.nteenviofnde = 'N' 
                    THEN '<span class=\"glyphicon glyphicon-thumbs-down\"></span>'
                    WHEN t.nteenviofnde = 'V' 
                    THEN '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
                     WHEN t.nteenviofnde = 'E' 
                    THEN '<span class=\"glyphicon glyphicon-thumbs-up\"></span>'
                END AS validar
            ";
                                }
                            } else {
                                $cabecalho = array(
                                    "Turma",
                                    "Nivel",
                                    "Situação");
                            }

                            $sql = "
            SELECT
                $acoesLista
                $cadeado
                '<a href=\"eja.php?modulo=principal/aluno/listar&acao=C&nteid='|| t.nteid ||'\">' || 'Turma ' || t.ntesequencial || '</a>' AS turma,
                nt.nivdescricao,
                CASE WHEN t.nteenviofnde = 'V' THEN
                    'Validada'
                WHEN t.ntefechaturma = 'A' THEN
                    'Aberta'
                    
                ELSE
                    'Fechada'
                END AS status
                $campoValidar
            FROM
                eja.novaturmaeja t
                JOIN eja.nivelturma nt ON t.nivid = nt.nivid
            WHERE
                t.ntestatus IN('A') -- Turma ativa ou fechada
                AND nteexercicio = '2015'
                {$filtroEscola}
            ORDER BY
                t.ntesequencial
        ";
                            echo $textoVal . '<br>';

                            $listagem = new Simec_Listagem();
                            $listagem->setCabecalho($cabecalho);
                            $listagem->setQuery($sql);
                            if (
                                    in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
                                    in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)
                            ) {
                                 if (strtotime($dataAtual) <= strtotime($dataLimiteVal) || $_SESSION['TipoUsuario']) {
                                $listagem->setAcoes(array(
                                    'delete' => 'deleteLimite'
                                ));
                                 }
                            }
                            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                            ?>

                            <?php if (false === $listagem->render()): ?>
                                <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                                    Nenhum registro encontrado
                                </div>
                            <?php endif; ?>

                            </div>

                            <div id="div_nivel_turma">
                                <div class="col-lg-12">
                                    <div class="well">
                                        <fieldset class="text-center">
                                            <div class="form-group">
                                                <label for="file" class="col-lg-5 control-label">
                                                    Selecione o nível da turma:
                                                </label>
                                                <div class="col-lg-2">
<?PHP
$nivelTurma = NULL;
$sql = "
                                    SELECT DISTINCT
                                        nivid AS codigo,
                                        nivdescricao AS descricao
                                    FROM
                                        eja.nivelturma
                                    ORDER BY
                                        descricao
                                ";
$db->monta_combo('nivelTurma', $sql, 'S', "Selecione...", '', '', '', '', 'N', 'nivelTurma', false, $nivelTurma, 'Selecione o nível da turma');
?>
                                                </div>
                                            </div>
                                        </fieldset>
                                    </div>
                                </div>
                            </div>

                            <div id="div_data_funcionamento">
                                <div class="col-lg-12">
                                    <div class="well">
                                        <fieldset class="text-center">
                                            <div class="form-group">
                                                <label for="file" class="col-lg-8 control-label">
                                                    Informe a data de início do funcionamento da turma na escola:
                                                </label>
                                                <div class="col-lg-3">
                                                    <input type="text" maxlength="12" class="normal form-control" name="dataFuncionamento" id="dataFuncionamento" value="" onblur="validando_data(this);"/>
                                                </div>
                                            </div>
                                        </fieldset>
                                    </div>
                                </div>
                            </div>



                            <script>
                                jQuery(document).ready(function() {
                                    modalDataLimite;
                                    jQuery('#dataFuncionamento').mask('99/99/9999');
                                    jQuery('#dataFuncionamento').datepicker({maxDate: 0});

                                    jQuery('#btnVoltar').click(function() {
                                        voltar();
                                    });

                                    jQuery('#btnInserir').click(function() {
                                        abrirDialogoNivelTurma();
                                    });

                                    jQuery('#uf').change(function() {
                                        jQuery('#formTurma').submit();
                                    });

                                    jQuery('#muncod').change(function() {
                                        jQuery('#formTurma').submit();
                                    });

                                    jQuery('#entid').change(function() {
                                        jQuery('#formTurma').submit();
                                    });

                                    jQuery('#div_nivel_turma').hide();
                                    jQuery('#div_data_funcionamento').hide();
                                });

                                function inserir() {
                                    window.location += '&requisicao=inserir';
                                }

                                function deleteLimite(nteid) {
                                    if (confirm("Deseja realmente excluir esta turma?")) {
                                        window.location += '&requisicao=excluir&nteid=' + nteid;
                                    }
                                }

                                function fechar(nteid) {
                                    window.location += '&requisicao=fechar&nteid=' + nteid;
                                }

                                function abrir(nteid) {
                                    window.location += '&requisicao=abrir&nteid=' + nteid;
                                }

                                function modalDataLimite() {
                                    $('#modal-confirm .modal-body p').html('Data Limite 19/01/2015.');
                                    $('#modal-confirm .modal-title').html('Aviso');
                                    $('#modal-confirm .btn-primary').remove();
                                    $('#modal-confirm .btn-default').html('Fechar');
                                    $('.modal-dialog').show();
                                    $('#modal-confirm').modal();
                                }

<?php
if (
        in_array(PERFIL_EJA_SUPER_USUARIO, $listaPerfil) ||
        in_array(PERFIL_EJA_GESTOR, $listaPerfil) ||
        in_array(PERFIL_EJA_GESTOR_ESTADUAL, $listaPerfil) ||
        in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $listaPerfil)
):
    ?>
                                    function validar(nteid) {
                                        window.location += '&requisicao=validar&nteid=' + nteid;
                                    }

                                    function naoValidar(nteid) {
                                        window.location += '&requisicao=naovalidar&nteid=' + nteid;
                                    }
<?php else: ?>
                                    function validar(nteid) {
                                    }
                                    function naoValidar(nteid) {
                                    }
<?php endif; ?>

                                function abrirDialogoNivelTurma() {
                                    $("#div_nivel_turma").dialog({
                                        title: "Nível da Turma",
                                        height: 300,
                                        width: 600,
                                        modal: true,
                                        resizable: true,
                                        closeOnEscape: true,
                                        show: {
                                            effect: "fade",
                                            duration: 500
                                        },
                                        hide: {
                                            effect: "fade",
                                            duration: 500
                                        },
                                        buttons: {
                                            Cancel: function() {
                                                jQuery(this).dialog("close");
                                            },
                                            "OK": function() {
                                                if (validarNivelTurma() == true) {
                                                    jQuery('#nivid').val(jQuery('#nivelTurma').val());
                                                    jQuery(this).dialog("close");
                                                    abrirDialogoFuncionamento();
                                                }
                                            }
                                        }
                                    });


                                    $('#nivelTurma').chosen();
                                }

                                function validarNivelTurma() {
                                    validacao = true;
                                    if (jQuery('#nivelTurma').val() == '') {
                                        alert('Atenção, o campo nível da turma é de preenchimento obrigatório!');
                                        validacao = false;
                                    }
                                    return validacao;
                                }

                                function abrirDialogoFuncionamento() {
                                    jQuery("#div_data_funcionamento").dialog({
                                        title: "Data de funcionamento da turma",
                                        height: 250,
                                        width: 750,
                                        modal: true,
                                        resizable: true,
                                        closeOnEscape: true,
                                        show: {
                                            effect: "fade",
                                            duration: 500
                                        },
                                        hide: {
                                            effect: "fade",
                                            duration: 500
                                        },
                                        buttons: {
                                            Cancel: function() {
                                                jQuery(this).dialog("close");
                                            },
                                            "OK": function() {
                                                if (validarDataFuncionamento() == true) {
                                                    jQuery('#ntedatainiturma').val(jQuery('#dataFuncionamento').val());
                                                    jQuery(this).dialog("close");
                                                    window.location += '&requisicao=salvar&entid=' + jQuery('#entid').val() +
                                                            '&nivid=' + jQuery('#nivid').val() +
                                                            '&ntedatainiturma=' + jQuery('#ntedatainiturma').val();
                                                }
                                            }
                                        }
                                    });
                                }

                                function validarDataFuncionamento() {
                                    var validacao = true;

                                    if (jQuery('#dataFuncionamento').val() == '') {
                                        alert('Atenção, o campo data de início do funcionamento da turma na escola é de preenchimento obrigatório!');
                                        validacao = false;
                                    }
                                    return validacao;
                                }

                            </script>