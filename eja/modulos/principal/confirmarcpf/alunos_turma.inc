<?php
include APPRAIZ . "includes/cabecalho.inc";
require (APPRAIZ . 'includes/library/simec/Listagem.php');
$perfis = pegaPerfilGeral();
$dataAtual = date('Y/m/d');
$dataLimite = date('2015/10/01');

if ($_REQUEST['requisicao'] == 'confirmarCadastro') {
    if (is_array($_REQUEST['ateid'][id])) {
        foreach ($_REQUEST['ateid'][id] as $key => $value) {
            $sqlCPF .= "UPDATE eja.alunoturmaeja
   SET atestatus2p='C'
 WHERE ateid = '{$key}';";
        }

        if ($sqlCPF) {
            $db->executar($sqlCPF);
            $db->commit();
        }

        if ($_REQUEST['nteid']) {
            $sqlCPFN = "UPDATE eja.alunoturmaeja
   SET atestatus2p='N'
 WHERE nteid = {$_REQUEST['nteid']} and (atestatus2p is null or atestatus2p = '')";
            $db->executar($sqlCPFN);
            $db->commit();
        }
    }else {
            $sqlCPFN = "UPDATE eja.alunoturmaeja
   SET atestatus2p='N'
 WHERE nteid = {$_REQUEST['nteid']} and (atestatus2p is null or atestatus2p = '')";
            $db->executar($sqlCPFN);
            $db->commit();
        }
        $sqlNTE = "UPDATE eja.novaturmaeja
   SET nteenviofnde2p='C'
 WHERE nteid = {$_REQUEST['nteid']}";
        $db->executar($sqlNTE);
        $db->commit();
    echo "<script>alert('Operação realizada com sucesso!'); window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A';</script>";
}

if (!in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) || (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && $_REQUEST['status'] == 'C' )) {

    function addCheckbox($id, $options) {
        $acao = <<<HTML
<div class="make-switch switch-mini" data-on-label="X" data-off-label="-" data-off="danger">
    <input type="checkbox" class="ckboxChild" name="ateid[id][%s]" disabled='disabled' />
</div>
HTML;
        return sprintf($acao, $id);
    }

} else {

    function addCheckbox($id, $options) {
        $acao = <<<HTML
<div class="make-switch switch-mini" data-on-label="X" data-off-label="-" data-off="danger">
    <input type="checkbox" class="ckboxChild" name="ateid[id][%s]" />
</div>
HTML;
        return sprintf($acao, $id);
    }

}
if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis)) {
    if($_REQUEST['entid']){
    $sqlDados = "SELECT
								 DISTINCT
                                                                 ent.entcodent as codigo,
								 ent.entnome as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								ur.usucpf = '{$_SESSION['usucpf']}' 
                                                                and ent.entid = '{$_REQUEST['entid']}'
								ORDER BY
								 ent.entnome ";
        
    $dadosP = $db->PegaLinha($sqlDados);
    }else{
        echo"<script>window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A';</script>";
    }
} else if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis) || in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis)) {
    $sqlDados = "SELECT
								 DISTINCT
                                                                 ent.entid as id,
                                                                 ent.entcodent as codigo,
								 ent.entnome as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								ent.entid = '{$_REQUEST['entid']}'
								ORDER BY
								 ent.entnome ";
    $dadosP = $db->PegaLinha($sqlDados);
}
?>
<script>
    $(document).ready(function() {
        $('#btnVoltar').click(function() {
<?php if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis) || in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis)) { ?>
                window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmas&entid=<?php echo $dadosP['id']; ?>';
<?php } else { ?>
                window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A';
<?php } ?>
        });
    });

    var ControleCheckbox = window.ControleCheckbox = {};
    ControleCheckbox = function(html) {
        this.el = $(html);
        this.dad = this.el.find("#tudo");
        this.child = this.el.find(".ckboxChild");
        this.ControllerCheckbox();
    };
    ControleCheckbox.prototype.ControllerCheckbox = function() {
        this.dad.on("click", $.proxy(this.SelectDad, this));
        this.child.on("click", $.proxy(this.SelectChild, this));
    };
    ControleCheckbox.prototype.SelectDad = function(e) {
        if (this.dad.prop("checked") === true) {
            this.markUnmark(true);
        } else {
            this.markUnmark(false);
        }
    };
    ControleCheckbox.prototype.SelectChild = function() {
        if (!this.child.is(":checked")) {
            this.dad.prop("checked", false);
        }

        if (this.child.length === this.childLenght) {
            this.dad.prop("checked", true);
        } else {
            this.dad.prop("checked", false);
        }
    };
    ControleCheckbox.prototype.markUnmark = function(flag) {
        this.child.each(function(index, element) {
            $(element).prop('checked', flag ? true : false);
        });
    };
    $(function() {
        new ControleCheckbox('#tb_render');
    });

    function confirmacao() {
        var resposta = confirm("Tem certeza que todos os CPFs foram verificados?");
        if (resposta == true) {
            var resposta2 = confirm("Após a confirmação os dados não poderão mais ser alterados! Tem certeza que todos os CPFs foram validados?");
            if (resposta2 == true) {
                $("#requisicao").val('confirmarCadastro');
                document.formPrincipal.submit();
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
</script>
<div class="page-header">
    <h4 id="forms">Confirmação de CPF</h4>   
</div>
<br>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <div class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">Código do INEP</label>
                <div class="col-lg-10">
                    <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['codigo']; ?>"readonly="readonly" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">Nome da Escola</label>
                <div class="col-lg-10">
                    <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['nome']; ?>"readonly="readonly" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">Endereço da Escola</label>
                <div class="col-lg-10">
                    <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['endereco']; ?>"readonly="readonly" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">UF</label>
                <div class="col-lg-10">
                    <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['uf']; ?>"readonly="readonly" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">Município</label>
                <div class="col-lg-10">
                    <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['mun']; ?>"readonly="readonly" />
                </div>
            </div>
        </fieldset>
    </div>
</div>
<br />
<?php
$listagem = new Simec_Listagem();
$cabecalho = array();
if (strtotime($dataAtual) >= strtotime($dataLimite)) {
    $sqlAluno = "        SELECT
           case WHEN a.atestatus2p = 'C' THEN '<b>Confirmado</b>' WHEN a.atestatus2p = 'E' THEN '<b>Confirmado - Lote Gerado</b>' WHEN a.atestatus2p = 'N' THEN '<font color=\"red\">Não Confirmado</font>' ELSE 'Prazo para confirmação/validação encerrado.'  END as status,
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
            a.atenomedoaluno";

    $cabecalho = array(
        "Status",
        "CPF",
        "Nome",
        "Sexo",
        "Data de Nascimento",
        "Nome da Mãe");
} else {
    if ((in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && ($_REQUEST['status'] == 'C' || $_REQUEST['status'] == 'E')) || (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && $_REQUEST['status'] == 'V') || in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis) || in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis)) {
        $sqlAluno = "        SELECT
           case WHEN a.atestatus2p = 'C' THEN '<b>Confirmado</b>' WHEN a.atestatus2p = 'E' THEN '<b>Confirmado - Lote Gerado</b>' WHEN a.atestatus2p = 'N' THEN '<font color=\"red\">Não Confirmado</font>' ELSE 'Aguardando Confirmação' END as status,
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
            a.atenomedoaluno";
        $cabecalho = array(
            "Status",
            "CPF",
            "Nome",
            "Sexo",
            "Data de Nascimento",
            "Nome da Mãe");
    } if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && !$_REQUEST['status']) {
        $sqlAluno = "        SELECT
            a.ateid as codigo,
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
            a.atenomedoaluno";
        if ($_REQUEST['status'] == 'C') {
            $cabecalho = array(
                "<input type='checkbox' name='tudo' id='tudo' disabled='disabled'/>",
                "CPF",
                "Nome",
                "Sexo",
                "Data de Nascimento",
                "Nome da Mãe");
        } else {
            //"<input type='checkbox' name='tudo' id='tudo'/>",
            $cabecalho = array(
                "",
                "CPF",
                "Nome",
                "Sexo",
                "Data de Nascimento",
                "Nome da Mãe");
        }
    }
    $listagem->addCallbackDeCampo('codigo', 'addCheckbox');
}
$listagem->setCabecalho($cabecalho);
$listagem->setQuery($sqlAluno);
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
?>
<form name="formPrincipal" id="formPrincipal" method="post" action="eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A">
    <input type="hidden" name="nteid" id="nteid" value="<?php echo $_REQUEST['nteid']; ?>">
    <input type="hidden" name="requisicao" id="requisicao">
    <?php if (false === $listagem->render()):
        ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>
</form>
<div class="form-group">
    <div class="col-lg-12" style="text-align: center;">
        <button class="btn btn-primary" id="btnVoltar" type="button" >Voltar</button>
        <?php
        if (strtotime($dataAtual) <= strtotime($dataLimite)) {
            ?>
            <button class="btn btn-success" id="btnCadastrar" type="button" <?php
        if ($_REQUEST['status'] == 'C' || $_REQUEST['status'] || !in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis)) {
            echo 'disabled';
        }
            ?> onclick="confirmacao()
                                    ;">Confirmar</button>
                <?php } ?>    
    </div>
</div>
