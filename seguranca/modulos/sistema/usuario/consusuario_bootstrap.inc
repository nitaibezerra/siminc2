<?php
/*
 * Sistema Simec Setor responsável: SPO-MEC Desenvolvedor:
 * Equipe Simec
 *
 * Analista: Werter Dias Almeida
 * Programador: Lindalberto Filho
 * Módulo:consusuario_bootstrap.inc
 * Finalidade: Atualização de interface (bootstrap)
 * Data: 21/07/2014
 */
#ver($_REQUEST);

require_once APPRAIZ . "includes/library/simec/Listagem.php";

$modulo = $_REQUEST['modulo'];
$encontrou = false;

if ($_REQUEST['act'] == 'visualizaXls') {
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
} else {
    $listagem = new Simec_Listagem();
}

if ($_SESSION['sisid'] != 4) {
    $habil = 'N';
    $_REQUEST['codigo'] = $_SESSION['sisid'];
} else
    $habil = 'S';


if ($_REQUEST['requisicaoAjax']) {
    $_REQUEST['requisicaoAjax']();
    exit;
}

include APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

if ($_REQUEST['acao'] == 'A') {
    $titulo_modulo = 'Alterar o Cadastro de Usuários do Sistema';
}
if ($_REQUEST['acao'] == 'C') {
    $titulo_modulo = 'Consultar o Cadastro de Usuários do Sistema';
}

//monta_titulo( $titulo_modulo, '' );

function mask($val) {
    $mask = '###.###.###-##';
    $maskared = '';
    $k = 0;
    for ($i = 0; $i <= strlen($mask) - 1; $i++) {
        if ($mask[$i] == '#') {
            if (isset($val[$k]))
                $maskared .= $val[$k++];
        }
        else {
            if (isset($mask[$i]))
                $maskared .= $mask[$i];
        }
    }
    return $maskared;
}

if ($_REQUEST['formulario']) {
    $usucpf = $_REQUEST['usucpf'];
    $usunome = $_REQUEST['usunome'];
    $codigo = $_REQUEST['codigo'];
    $suscod = $_REQUEST['suscod'];
    $pflcod = $_REQUEST['pflcod'];
    $muncod = $_REQUEST['muncod'];
    $usuchaveativacao = $_REQUEST['usuchaveativacao'];
    $sitperfil = $_REQUEST['sitperfil'];
    $unidade = $_REQUEST['unidade'];
} else {
    $codigo = $_SESSION['sisid'];
}

//Monta o combo com a listagem dos municípios após a escolha de um estado.
function listar_Municipios() {
    global $db;
    $sql = "SELECT muncod, mundescricao as mundsc
		FROM territorios.municipio
		WHERE estuf = '" . $_REQUEST['estuf'] . "' ORDER BY mundsc";

    $dados = $db->carregar($sql);

    if (!$dados)
        die();

    $enviar .="<option value=\"\"> Todos </option> ";
    foreach ($dados as $data) {
        $enviar .="<option title=\"{$data['mundsc']}\" ";
        if (isset($_REQUEST['municipio']) && $_REQUEST['municipio'] == $data['muncod'])
            $enviar .="selected=true ";
        $enviar .="value= " . $data['muncod'] . ">  " . simec_htmlentities($data['mundsc']) . " </option> ";
    }
    die($enviar);
}
?>
<script language="JavaScript" type="text/javascript">

    function listar_municipios(estuf, muncod)
    {
        var url = window.location + "&requisicaoAjax=listar_Municipios&estuf=" + estuf;
        if (muncod != "")
            url += "&municipio=" + muncod;

        $.post(url, function (data) {
            $('#municipio').html(data);
            $("#municipio").trigger("chosen:updated");
        });
        $('#muncod_on').css('display', 'inherit');
        $('#muncod_off').css('display', 'none');

    }

    function ProcuraUsuario(tipo) {
        if (document.formulario.usunome) {
            if (tipo == 'xls') {
                document.formulario.act.value = "visualizaXls";
            } else {
                document.formulario.act.value = "procura";
            }
            document.formulario.submit();
        }
    }

    function consultar_usu(usucpf) {
        window.location = "<?= $_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf=" + usucpf;
    }

    function alterar_usu(usucpf) {
        window.location = "<?= $_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=" + usucpf;
    }

    function SelPerfil() {
        if (document.formulario.pflcod.value == '') {
            alert('Selecione o Perfil antes de escolher a Situação Perfil.');
            document.formulario.sitperfil[0].checked = false;
            document.formulario.sitperfil[1].checked = false;
            document.formulario.sitperfil[2].checked = false;
        }
    }

    $(document).ready(function () {
        var mu = "<? echo $_REQUEST['muncod']; ?>";
        if ($('#unidadeF').val() != "") {
            listar_municipios($('#unidadeF').val(), mu);
        }
        $("[name=sitperfil]").click(function () {
            if ($('#cod').val() == '') {
                alert('Selecione o Perfil antes de escolher a Situação Perfil.');

                setTimeout(function () {
                    $("[name=sitperfil]").parent('label').removeClass("active");
                    $("#sitperfil").attr('checked', false);
                }, 50);
            }
        });
    });

</script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="disabled">Sistema</li>
        <li class="disabled">Usuários</li>
        <li class="active"><?= $titulo_modulo ?></li>
    </ol>
</section>
<section class="col-md-12">
    <form method="POST" role="form" class="form-horizontal well" name="formulario" id="formulario">
        <input type="hidden" name="formulario" value="1"/>
        <input type=hidden name="modulo" value="<?= $modulo ?>"/>
        <input type=hidden name="act" value="0"/>
        <section class="form-group">
            <label for="cpf" class="col-md-2 control-label">CPF (ou parte do CPF):</label>
            <section class="col-md-10">
                <?= inputTexto('usucpf', '', 'cpf', 14, false, array('masc' => '###.###.###-##')); ?>
            </section>
        </section>
        <section class="form-group">
            <label class="col-md-2 control-label" for="nome">
                Nome completo (ou parte do nome):
            </label>
            <section class="col-md-10">
                <?= inputTexto('usunome', $valor, 'nome', 50, false); ?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="unidadeF">
                Unidade Federativa:
            </label>
            <section class="col-md-10">
                <?php
                $regcod = $_REQUEST['regcod_disable'];
                $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
                inputCombo('regcod_disable', $sql, $regcod, 'unidadeF', array('acao' => 'listar_municipios'));
                ?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="municipio">
                Município:
            </label>
            <style>
                #municipio_chosen{
                    width: 100% !important;
                }
            </style>
            <section class="col-md-10">
                <div id="muncod_on" style="<?= isset($_REQUEST['muncod']) ? 'display:inherit' : 'display:none;'; ?>">
                    <select  id="municipio" name="muncod" class="campode100porcento form-control chosen-select"></select>
                </div>
                <div id="muncod_off" class="form-control-static" style="color:#909090;<?= isset($_REQUEST['muncod']) ? 'display:none' : 'display:inherit;'; ?>">Selecione uma Unidade Federativa.</div>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="sistema">
                Sistema:
            </label>
            <section class="col-md-10">
                <?php
                $sql = "select s.sisid as codigo, s.sisdsc as descricao from seguranca.sistema s where s.sisstatus='A'";
                inputCombo('codigo', $sql, $valor, 'sistema', array('titulo' => 'Qualquer (selecione)', 'habil' => 'N'));
                ?>
            </section>
        </section>

        <?php if ($codigo) { ?>
            <section class="form-group">
                <label class="col-md-2 control-label" for="cod">
                    Perfil:
                </label>
                <section class="col-md-10">
                    <?php
                    $sql = <<<DML
                    	SELECT
							pflcod AS codigo,
							pfldsc AS descricao
						FROM seguranca.perfil
						WHERE pflstatus = 'A'
							AND sisid = $codigo
						ORDER BY pfldsc
DML;

                    $pflcod = $_POST['pflcod'];
                    inputCombo('pflcod', $sql, $valor, 'cod', array('titulo' => 'Todos'));
                    ?>
                </section>
            </section>
            <section class="form-group">
                <label class="col-md-2 control-label">Unidade Orçamentária:</label>
                <section class="col-md-10">
                    <?php
                    $sql = "
                                SELECT
                                    unicod as codigo,
                                    unicod || ' - ' || unidsc as descricao
                                    FROM public.unidade
                                    where orgcod = '". CODIGO_ORGAO_SISTEMA. "' and unistatus = 'A'
                                    ORDER BY unicod
";
                    $options = array(
                        'titulo' => 'Selecione o(s) Perfil(s)'
                    );
                    inputCombo('unidade[]', $sql, $unidade, 'unidade', $options);
                    ?>
                </section>
            </section>

        <?php } ?>

        <? if (!isset($_REQUEST['formulario'])) $suscod = 'A'; ?>
        <?php if ($_REQUEST['acao'] == 'A' || $_REQUEST['acao'] == 'C') { ?>
            <section class="form-group control-group">
                <label class="control-label col-md-2">
                    Status geral do usuário:
                </label>
                <section class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default <?php
                    if ($suscod == '') {
                        echo 'active';
                    }
                    ?>">
                        <input id="status_qualquer" type="radio" name="suscod" value="" <?= $suscod == '' ? 'checked="checked"' : "" ?>/>
                        Qualquer
                    </label>
                    <label for="status_ativo" class="btn btn-default <?php
                    if ($suscod == 'A') {
                        echo 'active';
                    }
                    ?>">
                        <input id="status_ativo" type="radio" name="suscod" value="A" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
                        Ativo
                    </label>
                    <label for="status_pendente" class="btn btn-default <?php
                    if ($suscod == 'P') {
                        echo 'active';
                    }
                    ?>">
                        <input id="status_pendente" type="radio" name="suscod" value="P" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
                        Pendente
                    </label>
                    <label for="status_bloqueado" class="btn btn-default <?php
                    if ($suscod == 'B') {
                        echo 'active';
                    }
                    ?>">
                        <input id="status_bloqueado" type="radio" name="suscod" value="B" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
                        Bloqueado
                    </label>
                </section>
            </section>
        <?php } ?>

        <section class="form-group">
            <label class="control-label col-md-2">Já acessou o sistema:</label>
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php
                if ($usuchaveativacao == '') {
                    echo 'active';
                }
                ?>">
                    <input id="acesso_qualquer" type="radio" name="usuchaveativacao" value="" <?= $usuchaveativacao == '' ? 'checked="checked"' : "" ?>/>
                    Qualquer
                </label>

                <label class="btn btn-default <?php
                if ($usuchaveativacao == 't') {
                    echo 'active';
                }
                ?>">
                    <input id="acesso_sim" type="radio" name="usuchaveativacao" value="t" <?= $usuchaveativacao == 't' ? 'checked="checked"' : "" ?>/>
                    Sim
                </label>

                <label class="btn btn-default <?php
                if ($usuchaveativacao == 'f') {
                    echo 'active';
                }
                ?>">
                    <input id="acesso_nao" type="radio" name="usuchaveativacao" value="f" <?= $usuchaveativacao == 'f' ? 'checked="checked"' : "" ?>/>
                    Não
                </label>
            </section>
        </section>
        <section class="form-group">
            <label class="control-label col-md-2">	Situação Perfil:</label>
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php
                if ($sitperfil == 'V') {
                    echo 'active';
                }
                ?>" name="sitperfil">
                    <input type="radio" name="sitperfil" id="sitperfil" value="V"  <?= $sitperfil == 'V' ? 'checked="checked"' : "" ?>/>
                    Qualquer
                </label>
                <label class="btn btn-default <?php
                if ($sitperfil == 'D') {
                    echo 'active';
                }
                ?>" name="sitperfil">
                    <input  type="radio" name="sitperfil" id="sitperfil" value="D"  <?= $sitperfil == 'D' ? 'checked="checked"' : "" ?>/>
                    Desejado
                </label>
                <label class="btn btn-default <?php
                if ($sitperfil == 'A') {
                    echo 'active';
                }
                ?>" name="sitperfil">
                    <input type="radio" name="sitperfil" id="sitperfil" value="A"  <?= $sitperfil == 'A' ? 'checked="checked"' : "" ?>/>
                    Atribuído
                </label>
            </section>
        </section>
        <section class="col-md-offset-2">
            <input type='button' class="btn btn-primary" name='consultar' value='Consultar' onclick="ProcuraUsuario('')">
            <input type='button' class="btn btn-danger" name='visualizarXls' value='Exportar XLS' onclick="ProcuraUsuario('xls')">
        </section>
        <div style="clear:both;"></div>
</section>
</form>
<section class="col-md-12">
    <?php
    unset($sql);
    // insere filtros de consulta
    if ($_REQUEST['usunome'])
        $texto = " and TRANSLATE(usuario.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%" . $_REQUEST['usunome'] . "%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ";
    if ($_REQUEST['usucpf'])
        $texto.= " and usuario.usucpf like '%" . str_to_upper(corrige_cpf($_REQUEST['usucpf'])) . "%'";

    if ($_REQUEST['suscod'])
        $texto.= " and usuariosistema.suscod = '" . $_REQUEST['suscod'] . "'";
    if ($_REQUEST['codigo'])
        $texto.= " and usuariosistema.sisid = '" . $_REQUEST['codigo'] . "'";
    /* UF e Municipio */
    if ($_REQUEST['regcod_disable'])
        $texto.= " and usuario.regcod = '" . $_REQUEST['regcod_disable'] . "'";
    if ($_REQUEST['muncod'])
        $texto.= " and usuario.muncod = '" . $_REQUEST['muncod'] . "'";
    //Unidade Orcamentaria
    if (!empty($_REQUEST['unidade'][0])) {
        foreach ($_REQUEST['unidade'] as $unidade) {
            $und[] = "unidadex.unicod = '" . (integer) $unidade . "'";
        }
        if (!empty($und)) {
            $texto .= ' AND ' . implode(' OR ', $und);
        }
    }
    // Se já acessou o sistema
    if ($_REQUEST["usuchaveativacao"])
        $texto .= " and usuario.usuchaveativacao = '" . $_REQUEST["usuchaveativacao"] . "'";

    if ($_REQUEST["sitperfil"] == '') {
        //$_REQUEST["sitperfil"] = 'V';
        $_REQUEST["sitperfil"] = 'A';
    }

    // verifica situação do perfil (Desejado ou atribuido)
    if ($_REQUEST["sitperfil"] == 'D') {
        if ($_REQUEST["pflcod"])
            $texto .= " and (usuariosistema.pflcod = " . $_REQUEST["pflcod"] . ")";
    }
    else if ($_REQUEST["sitperfil"] == 'A') {
        if ($_REQUEST["pflcod"])
            $texto .= " and (perfil.pflcod = " . $_REQUEST["pflcod"] . ")";
    }
    else if ($_REQUEST["sitperfil"] == 'V') {
        if ($_REQUEST["pflcod"])
            $texto .= " and (perfil.pflcod = " . $_REQUEST["pflcod"] . " OR usuariosistema.pflcod = " . $_REQUEST["pflcod"] . ")";
    }


    if ($arrCampo) {
        foreach ($arrCampo as $campo => $valor) {
            if ($campo && is_array($valor)) {
                $arrWhere[] = "resp.$campo in ('" . implode("','", $valor) . "') ";
            }
        }
        $arrWhere[] = "resp.rpustatus = 'A'";
        $innerResponsabilidade = " INNER JOIN {$_SESSION['sisdiretorio']}.usuarioresponsabilidade resp ON usuario.usucpf = resp.usucpf " . ($arrWhere ? " AND ( " . implode(" AND ", $arrWhere) . " ) " : "");
    }

    if ($_REQUEST['pflcod']) {
        $oin_sql = sprintf(
                "seguranca.perfil perfil
			inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = %d
			right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
			left join
			(select	unicod,	unidsc from	public.unidade  where unitpocod = 'U') as unidadex on usuario.unicod = unidadex.unicod", $_REQUEST['pflcod']
        );
    } else {
        $oin_sql = "
			( select unicod, unidsc from public.unidade  where	unitpocod = 'U') as unidadex
			right join seguranca.usuario usuario	on usuario.unicod = unidadex.unicod
			left join seguranca.perfilusuario perfilusuario on usuario.usucpf = perfilusuario.usucpf
			left join seguranca.perfil perfil on perfil.pflcod = perfilusuario.pflcod
			";
    }

    // seleciona qual ação o botão que aparece na listagem deve acionar
    if ($_REQUEST['acao'] == 'A') {
        $funcao = "alterar_usu";
    } else {
        $funcao = "consultar_usu";
    }

    $campos = "
        usuario.usucpf,
        usuario.usucpf AS cpf,
        usuario.usunome AS nomeusuario,
        '(' || usuario.usufoneddd || ') ' || usuario.usufonenum AS fone,
        CASE WHEN entidade.entid = 390402 THEN trim(usuario.orgao) ELSE trim(unidadex.unidsc) END AS unidsc,
        '<section class=\"btn-group\">' ||
        CASE WHEN usuariosistema.suscod = 'A' THEN
            '<label class=\"btn btn-default active\">Ativo</label>'
        WHEN usuariosistema.suscod = 'P' THEN
            '<label class=\"btn btn-warning active\">Pendente</label>'
        WHEN usuariosistema.suscod = 'B' THEN
            '<label class=\"btn btn-danger active\">Bloqueado</label>'
        ELSE
            '<label class=\"btn btn-danger active\">Inativo</label>'
        END ||
        '</section>' AS status,
        to_char(usuario.usudataatualizacao,'dd/mm/YYYY HH24:MI') AS data
    ";

    $groupby = "usuario.usucpf, usuario.usunome, usuario.usufoneddd, usuario.usufonenum, usuario.usuemail, entidade.entid,
                usuario.orgao, unidadex.unidsc, cargo.cardsc, usuario.usufuncao, entidade.entnome, usuario.regcod,
                municipio.mundescricao, usuario.usudataatualizacao, usuariosistema.suscod, usuario.usuchaveativacao, perfil.pflcod";

    $sql = "
        SELECT DISTINCT
            $campos
        FROM
            " . $oin_sql . "
            left join  territorios.municipio municipio on municipio.muncod = usuario.muncod
            inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
            left join  entidade.entidade entidade on usuario.entid = entidade.entid
            left join  public.cargo cargo on cargo.carid = usuario.carid
            $innerResponsabilidade
        WHERE
            usunome is not null $texto $andPerfilPermissao
        GROUP BY
            $groupby
        ORDER BY
            nomeusuario";

    $cabecalho = array(
        'CPF',
        'Nome Completo',
        'Telefone',
        'Unidade',
        'Status',
        'Data da Ultima Atualização');

    /* Exibe a consulta para um XLS */
    if ($_REQUEST['act'] == 'visualizaXls') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        ob_clean();
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColuna(array('usucpf'));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render();
        header("Content-Disposition: attachment; filename=consusuario.xls");
        die();
    }
    #ver($sql);
    /* Exibe a consulta na tela */
    $listagem->setFormFiltros('formulario');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->setAcoes(array('edit' => $funcao));
    $listagem->addCallbackDeCampo('cpf', 'mask');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>

</section>