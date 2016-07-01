<?php
error_reporting(0);

include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once "classes/Participante.class.inc";

$db = new cls_banco();

switch ($_REQUEST['action'])
{
    case 'carregarMunicipio':

        $ufs = isset($_REQUEST['estuf']) && is_array($_REQUEST['estuf']) ? " '" . implode ("', '", $_REQUEST['estuf']) . "'" : "''";        
        $sql = "select muncod, estuf || ' - ' || mundescricao as descricao
                                                from territorios.municipio
                                                where estuf in ({$ufs})
                                                order by estuf, mundescricao";

        $municipios = $db->carregar($sql);
        $municipios = $municipios ? $municipios : array();
        ?>

        <select name="muncod[]" id="muncod" class="form-control chosen-select" multiple data-placeholder="Selecione">
            <?php foreach ($municipios as $dado) { ?>
                <option <?php echo is_array($_POST['muncod']) && in_array($dado['muncod'], $_POST['muncod']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['muncod']; ?>"><?php echo $dado['descricao']; ?></option>
            <?php } ?>
        </select>

        <?php
        die;
}

$where = 'where true';
$join = array();
if (isset($_POST['estuf']) && is_array($_POST['estuf'])) {
    $where .= " and p.estuf in ('" . implode ("', '", $_POST['estuf']) . "') ";
}
if (isset($_POST['muncod']) && is_array($_POST['muncod'])) {
    $where .= " and p.muncod in ('" . implode ("', '", $_POST['muncod']) . "') ";
}
if (isset($_POST['iteid']) && is_array($_POST['iteid'])) {
    $where .= " and a.iteid in (" . implode (", ", $_POST['iteid']) . ") ";
    $join[] = ' inner join consultapne.avaliacao a on a.queid = q.queid ';
}
if (isset($_POST['avaresposta']) && is_array($_POST['avaresposta'])) {
    $where .= " and a.avaresposta in (" . implode (", ", $_POST['avaresposta']) . ") ";
    $join[] = ' inner join consultapne.avaliacao a on a.queid = q.queid ';
}
if (isset($_POST['quesituacao']) && is_array($_POST['quesituacao'])) {
    $where .= " and q.quesituacao in ('" . implode ("', '", $_POST['quesituacao']) . "') ";
}
if (isset($_POST['tpoid']) && is_array($_POST['tpoid'])) {
    $where .= " and p.tpoid in (" . implode (", ", $_POST['tpoid']) . ") ";
}
if (isset($_POST['parrepresentacao']) && is_array($_POST['parrepresentacao'])) {
    $where .= " and p.parrepresentacao in (" . implode (", ", $_POST['parrepresentacao']) . ") ";
}
if (!empty($_POST['comid'])) {
    $join[] = ' inner join consultapne.avaliacao a on a.queid = q.queid ';
    $join[] = ' inner join consultapne.comentario c on c.avaid = a.avaid ';
    $where .= " and coalesce(c.comid, 0) != 0 ";
}
if (!empty($_POST['data_inicio']) || !empty($_POST['data_fim'])) {
    if(!empty($_POST['data_inicio']) && !empty($_POST['data_fim'])){
        $dtInicio = formata_data_sql($_POST['data_inicio']);
        $dtFim = formata_data_sql($_POST['data_fim']);
        $where .= " and to_char(q.quedtfinalizacao, 'YYYY-MM-DD') between '$dtInicio' and '$dtFim'";
    } elseif(!empty($_POST['data_inicio'])) {
        $dtInicio = formata_data_sql($_POST['data_inicio']);
        $where .= " and to_char(q.quedtfinalizacao, 'YYYY-MM-DD') >= '$dtInicio'";
    } else {
        $dtFim = formata_data_sql($_POST['data_fim']);
        $where .= " and to_char(q.quedtfinalizacao, 'YYYY-MM-DD') <= '$dtFim'";
    }
}
if (!empty($_POST['nome'])) {
    $where .= " and (
                                p.parnome ilike ('%{$_POST['nome']}%') OR
                                p.parreprazaosocial ilike ('%{$_POST['nome']}%') OR
                                p.parrepnomefantasia ilike ('%{$_POST['nome']}%') OR
                                p.parorgao ilike ('%{$_POST['nome']}%')
                              )";
}
if (!empty($_POST['cpf'])) {
    $cpf = ereg_replace("[^0-9]","",$_POST['cpf']);
    $where .= " and (
                                p.parcpf ilike ('%{$cpf}%') OR
                                p.parcnpj ilike ('%{$cpf}%')
                            )";
}
$join = array_unique($join);

$_SESSION['sisid'] = 4;
$participante = new Participante();

if($_POST['excel']){

    $sql = "select p.parid, i.itedsc as Artigo,
                case
                    when avaresposta = 5 then 'Concordo totalmente'
                    when avaresposta = 4 then 'Concordo parcialmente'
                    when avaresposta = 3 then 'Não concordo e nem discordo'
                    when avaresposta = 2 then 'Discordo parcialmente'
                    when avaresposta = 1 then 'Discordo totalmente'
                    else ''
                    end as descricao,
                parcpf as cpf, parnome as Nome, p.estuf as UF, m.mundescricao as Município, parsexo as Sexo,
                case
                    when parrepresentacao = 1 then 'Órgão, Entidade ou Insituição PÚBLICA'
                    when parrepresentacao = 2 then 'Órgão, Entidade ou Insituição PRIVADA'
                    when parrepresentacao = 3 then 'Pseeoa Física'
                    else 'Outros'
                end as \"Representação\",
                parcnpj as CNPJ, parreprazaosocial \"Razão Social\", parrepnomefantasia as \"Nome Fantasia\",
                comdsc as Comentário
            from consultapne.item i
                left join consultapne.avaliacao a on i.iteid = a.iteid
                left join consultapne.comentario c on a.avaid = c.avaid
                left join consultapne.questionario q on a.queid = q.queid
                left join consultapne.participante p on q.parid = p.parid
                left join territorios.municipio m on p.muncod = m.muncod
            {$where}
            and i.itetipo = 'P'
            group by i.iteid, i.iteordem, avaresposta, parnome, p.parid,parcpf, m.mundescricao,parsexo,comdsc
            order by i.iteid, i.iteordem, parnome, avaresposta desc";
    //ver($sql,d);
    ob_clean();
    header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
    header ( "Pragma: no-cache" );
    header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
    header ( "Content-Disposition: attachment; filename=rel_contatos_".date("Ymdhis").".xls");
    header ( "Content-Description: MID Gera excel" );
    $db->monta_lista_tabulado($sql,array('Identificador Interno', 'Artigo', 'Resposta', 'CPF', 'Nome', 'UF', 'Município', 'Sexo', 'Representação', 'CNPJ', 'Razão Social', 'Nome Fantasia', 'Comentário'),100000000,5,'N','100%','');
    exit;
}


?>

<!DOCTYPE html>
<html lang="pt-BR">
    <?php require "head.php"; ?>

    <script src="js/jquery.min.js"></script>
    <script src="js/bootstrap.min.js"></script>
    <script src="js/jquery.chosen.js" type="text/javascript"></script>
    <script src="js/jquery.maskedinput.min.js" type="text/javascript"></script>
    <style>
        body {
            background-image: url("imagens/bg.png");
        }
    </style>
    <header>
        <div class="row">
            <div class="col-lg-12 col-sm-12 col-xs-12">
            	<div style="width: 180px; float: left;">
                    <img src="imagens/logo-simec.png" class="res" width="150">
                </div>
                <div style="width: 300px; float: left; text-align: left">
                	<div class="countdown-container data-termino"></div>
                </div>
                <div style="width: 180px; float: right;">
                	<a class="brasil pull-right" href="http://www.brasil.gov.br/"><img alt="Brasil - Governo Federal" src="/estrutura/temas/default/img/brasil.png" style="margin-right: 10px;"></a>
                </div>
            </div>
        </div>
    </header>

    <div class="container">
        <div class="row">
            <div class="col-lg-12 col-sm-12 col-xs-12">
                <h2 style="text-align: center;">Política Nacional de Formação dos Profissionais da Educação Básica</h2>
            </div>
        </div>

        <div class="row">
            <div class="panel panel-success">
                <div class="panel-heading">
                    <strong>Filtro</strong>
                </div>
                <div class="panel-body">

                    <form action="" name="formulario" id="formulario" class="form-horizontal" method="post">
                        <div style="margin-bottom: 20px;">
                            <table>
                                <tr>
                                    <?php
                                    $sql = "SELECT	estuf, estdescricao	FROM territorios.estado	ORDER BY estuf ";
                                    $arrDados = $db->carregar($sql);

                                    foreach ($arrDados as $dados) {
                                        $active = is_array($_POST['estuf']) && in_array($dados['estuf'], $_POST['estuf']);
                                        ?>
                                        <td>
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default <?php echo $active ? 'active' : ''; ?>">
                                                    <input type="checkbox" class="checkbox-uf" name="estuf[]" autocomplete="off" value="<?php echo $dados['estuf'];?>" <?php echo $active ? 'checked="checked"' : ''; ?>>
                                                    <img width="15px" src="/imagens/bandeiras/mini/<?php echo $dados['estuf']; ?>.png"><br>
                                                    <div style="font-size: 10px">
                                                        <?php echo $dados['estuf'];?>
                                                    </div>
                                                </label>
                                            </div>
                                        </td>
                                    <?php } ?>
                                </tr>
                            </table>
                        </div>


                        <div class="form-group">
                            <label for="nome" class="col-sm-2 control-label">Nome:</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="nome" name="nome" placeholder="Nome da pessoa ou da Instituição" value="<?php echo !empty($_POST['nome']) ? $_POST['nome'] : ''; ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="cpf" class="col-sm-2 control-label">CPF/CNPJ:</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="cpf" name="cpf" placeholder="CPF da pessoa ou CNPJ da Instituição" value="<?php echo !empty($_POST['cpf']) ? $_POST['cpf'] : ''; ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="cpf" class="col-sm-2 control-label">Município:</label>
                                <?php
                                $ufs = isset($_POST['estuf']) && is_array($_POST['estuf']) ? " '" . implode ("', '", $_POST['estuf']) . "'" : "''";

                                $sql = "select muncod, estuf || ' - ' || mundescricao as descricao
                                        from territorios.municipio
                                        where estuf in ({$ufs})
                                        order by estuf, mundescricao";
                                $municipios = $db->carregar($sql);
                                $municipios = $municipios ? $municipios : array();
                                ?>

                            <div class="col-sm-10" id="div_municipio">
                                <select name="muncod[]" id="muncod" class="form-control chosen-select" multiple data-placeholder="Selecione">
                                    <?php foreach ($municipios as $dado) { ?>
                                        <option <?php echo is_array($_POST['muncod']) && in_array($dado['muncod'], $_POST['muncod']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['muncod']; ?>"><?php echo $dado['descricao']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="cpf" class="col-sm-2 control-label">Artigo:</label>
                            <div class="col-sm-10">
                                <?php
                                $sql = "select iteid, replace(substring(itedsc, 0, 100), '-', '') || '...' as itedsc
                                        from consultapne.item i
                                        where itetipo = 'P'
                                        order by i.iteid, iteordem";
                                $itens = $db->carregar($sql);                                 
                                ?>

                                <select name="iteid[]" id="iteid[]" class="form-control chosen-select" multiple data-placeholder="Selecione">
                                    <?php foreach ($itens as $dado) { ?>
                                        ver($itens,d);
                                        <option <?php echo is_array($_POST['iteid']) && in_array($dado['iteid'], $_POST['iteid']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['iteid']; ?>"><?php echo $dado['itedsc']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="quesituacao" class="col-sm-2 control-label">Período Finalização:</label>
                            <div class="col-sm-10">
                                <input  type="text" class="data form-control pull-left" style="width: 150px" id="data_inicio" name="data_inicio" value="<?php echo !empty($_POST['data_inicio']) ? $_POST['data_inicio'] : ''; ?>">
                                <span class="pull-left" style="margin: 5px;">a </span>
                                <input type="text" class="data form-control" style="width: 150px" id="data_fim" name="data_fim" value="<?php echo !empty($_POST['data_fim']) ? $_POST['data_fim'] : ''; ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="quesituacao" class="col-sm-2 control-label">Situação:</label>
                            <div class="col-sm-10">
                                <div class="btn-group " data-toggle="buttons">
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['quesituacao']) && in_array('F', $_POST['quesituacao']) ? 'active' : null; ?>">
                                        <input name="quesituacao[]" class="raty" type="checkbox" value="F" <?php echo is_array($_POST['quesituacao']) && in_array('F', $_POST['quesituacao']) ? 'checked="checked"' : ''; ?>> Finalizada
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['quesituacao']) && in_array('A', $_POST['quesituacao']) ? 'active' : null; ?>">
                                        <input name="quesituacao[]" class="raty" type="checkbox" value="A" <?php echo is_array($_POST['quesituacao']) && is_array($_POST['quesituacao']) && in_array('A', $_POST['quesituacao']) ? 'checked="checked"' : ''; ?>> Em Preenchimento
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="inputPassword3" class="col-sm-2 control-label">Tipo Órgão:</label>
                            <div class="col-sm-10">
                                <div class="btn-group " data-toggle="buttons">
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['tpoid']) && in_array('1', $_POST['tpoid']) ? 'active' : null; ?>">
                                        <input name="tpoid[]" class="raty" type="checkbox" value="1" <?php echo is_array($_POST['tpoid']) && in_array('1', $_POST['tpoid']) ? 'checked="checked"' : ''; ?>> Federal
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['tpoid']) && in_array('2', $_POST['tpoid']) ? 'active' : null; ?>">
                                        <input name="tpoid[]" class="raty" type="checkbox" value="2" <?php echo is_array($_POST['tpoid']) && in_array('2', $_POST['tpoid']) ? 'checked="checked"' : ''; ?>> Estadual
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['tpoid']) && in_array('3', $_POST['tpoid']) ? 'active' : null; ?>">
                                        <input name="tpoid[]" class="raty" type="checkbox" value="3" <?php echo is_array($_POST['tpoid']) && in_array('3', $_POST['tpoid']) ? 'checked="checked"' : ''; ?>> Municipal
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['tpoid']) && in_array('4', $_POST['tpoid']) ? 'active' : null; ?>">
                                        <input name="tpoid[]" class="raty" type="checkbox" value="4" <?php echo is_array($_POST['tpoid']) && in_array('4', $_POST['tpoid']) ? 'checked="checked"' : ''; ?>> Outros
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="inputPassword3" class="col-sm-2 control-label">Tipo Representação:</label>
                            <div class="col-sm-10">
                                <div class="btn-group " data-toggle="buttons">
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['parrepresentacao']) && in_array('1', $_POST['parrepresentacao']) ? 'active' : null; ?>">
                                        <input name="parrepresentacao[]" class="raty" type="checkbox" value="1" <?php echo is_array($_POST['parrepresentacao']) && in_array('1', $_POST['parrepresentacao']) ? 'checked="checked"' : ''; ?>> Órgão, Entidade ou Insituição PÚBLICA
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['parrepresentacao']) && in_array('2', $_POST['parrepresentacao']) ? 'active' : null; ?>">
                                        <input name="parrepresentacao[]" class="raty" type="checkbox" value="2" <?php echo is_array($_POST['parrepresentacao']) && in_array('2', $_POST['parrepresentacao']) ? 'checked="checked"' : ''; ?>> Órgão, Entidade ou Insituição PRIVADA
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['parrepresentacao']) && in_array('3', $_POST['parrepresentacao']) ? 'active' : null; ?>">
                                        <input name="parrepresentacao[]" class="raty" type="checkbox" value="3" <?php echo is_array($_POST['parrepresentacao']) && in_array('3', $_POST['parrepresentacao']) ? 'checked="checked"' : ''; ?>> Pessoa Física
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="inputPassword3" class="col-sm-2 control-label">Resposta:</label>
                            <div class="col-sm-10">
                                <div class="btn-group " data-toggle="buttons">
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['avaresposta']) && in_array('5', $_POST['avaresposta']) ? 'active' : null; ?>">
                                        <input name="avaresposta[]" class="raty" type="checkbox" value="5" <?php echo is_array($_POST['avaresposta']) && in_array('5', $_POST['avaresposta']) ? 'checked="checked"' : ''; ?>> Concordo totalmente
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['avaresposta']) && in_array('4', $_POST['avaresposta']) ? 'active' : null; ?>">
                                        <input name="avaresposta[]" class="raty" type="checkbox" value="4" <?php echo is_array($_POST['avaresposta']) && in_array('4', $_POST['avaresposta']) ? 'checked="checked"' : ''; ?>> Concordo parcialmente
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['avaresposta']) && in_array('3', $_POST['avaresposta']) ? 'active' : null; ?>">
                                        <input name="avaresposta[]" class="raty" type="checkbox" value="3" <?php echo is_array($_POST['avaresposta']) && in_array('3', $_POST['avaresposta']) ? 'checked="checked"' : ''; ?>> Não concordo e nem discordo
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['avaresposta']) && in_array('2', $_POST['avaresposta']) ? 'active' : null; ?>">
                                        <input name="avaresposta[]" class="raty" type="checkbox" value="2" <?php echo is_array($_POST['avaresposta']) && in_array('2', $_POST['avaresposta']) ? 'checked="checked"' : ''; ?>> Discordo parcialmente
                                    </label>
                                    <label class="btn btn-raty btn-default <?php echo is_array($_POST['avaresposta']) && in_array('1', $_POST['avaresposta']) ? 'active' : null; ?>">
                                        <input name="avaresposta[]" class="raty" type="checkbox" value="1" <?php echo is_array($_POST['avaresposta']) && in_array('1', $_POST['avaresposta']) ? 'checked="checked"' : ''; ?>> Discordo totalmente
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-2 col-sm-10">
                                <div class="checkbox">
                                    <label>
                                        <input type="checkbox" name="comid" value="1" <?php echo !empty($_POST['comid']) ? 'checked="checked"' : ''; ?>> Somente participações com comentários
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-offset-2">
                            <input type="submit" name="enviar" class="btn btn-success" value="Pesquisar"/>
                            <input type="submit" name="excel" class="btn btn-primary" value="Gerar Excel"/>
                            <input type="button" name="Limpar" class="btn btn-warning" value="Limpar Filtros" id="limpar_filtro"/>
                        </div>

                    </form>
                </div>
            </div>
        </div>


        <div class="row">
            <?php

            $sql = "select distinct q.queid, p.parcpf, p.parnome, m.mundescricao, p.estuf, p.parcnpj, p.parreprazaosocial,
                            case
                                when p.parrepresentacao = 1 then '<span style=\"color: green;\">Pública</span>'
                                when p.parrepresentacao = 2 then '<span style=\"color: red;\">Privada</span>'
                                else 'Pessoa Física'
                            end as representacao,
                            case
                                when q.quesituacao = 'F' then '<span style=\"color: green;\">FINALIZADO</span>'
                                else '<span style=\"color: red;\">EM PREENCHIMENTO</span>'
                            end as situacao
                    from consultapne.questionario q
                        inner join consultapne.participante p on p.parid = q.parid
                        " . implode(' ', $join).  "
                        left join territorios.municipio m on m.muncod = p.muncod
                    {$where}
                    order by p.estuf, m.mundescricao, parnome
                     -- limit 10
                    "
                    ;

            $dados  = $participante->carregar($sql);
            $dados = $dados ? $dados : array();
            $total = is_array($dados) ? count($dados ) : 0;

            ?>

            <h2>Total de Contribuições: <span style="color: red;"><?php echo count($dados); ?></span></h2>

            <?php
                $sqlSituacao = "select  count(*) as valor,
                            case
                                when quesituacao = 'F' then 'FINALIZADO'
                                else 'EM PREENCHIMENTO'
                            end as descricao
                        from consultapne.questionario
                        group by descricao";

                $sqlRepresentacao = "select  count(*) as valor,
                                    case
                                        when parrepresentacao = 1 then 'Pública'
                                        when parrepresentacao = 2 then 'Privada'
                                        when parrepresentacao = 3 then 'CPF'
                                    else 'N/I'
                                    end as descricao
                                from consultapne.questionario q
                                    inner join consultapne.participante p on p.parid = q.parid
                                group by descricao
                                order by descricao desc";

                $sqlResposta = "select count(*) as valor,
                                    case
                                        when avaresposta = 5 then 'Concordo totalmente'
                                        when avaresposta = 4 then 'Concordo parcialmente'
                                        when avaresposta = 3 then 'Não concordo e nem discordo'
                                        when avaresposta = 2 then 'Discordo parcialmente'
                                        when avaresposta = 1 then 'Discordo totalmente'
                                        else 'NÃO INFORMADO'
                                    end as descricao
                                from consultapne.avaliacao
                                where coalesce(avaresposta, 0) != 0
                                GROUP BY descricao, avaresposta
                                order by avaresposta desc ";

                $sqlTipoItem = "select count(*) as valor, replace(substring(itedsc, 0, 10), '-', '') as categoria,
                                    case
                                        when avaresposta = 5 then 'Concordo totalmente'
                                        when avaresposta = 4 then 'Concordo parcialmente'
                                        when avaresposta = 3 then 'Não concordo e nem discordo'
                                        when avaresposta = 2 then 'Discordo parcialmente'
                                        when avaresposta = 1 then 'Discordo totalmente'
                                        else 'NÃO INFORMADO'
                                    end as descricao
                                from consultapne.avaliacao a
                                    inner join consultapne.item i on i.iteid = a.iteid
                                where itetipo = 'P'
                                and coalesce(avaresposta, 0) != 0
                                GROUP BY descricao, avaresposta, itedsc, i.iteid, iteordem
                                order by i.iteid, iteordem, avaresposta desc ";

            $sqlEstados = "select  count(*) as valor, 'Estados' as descricao, estuf as categoria
                            from consultapne.questionario q
                                inner join consultapne.participante p on p.parid = q.parid
                            group by estuf
                            order by estuf";

                $grafico = new Grafico();
            ?>

            <div class="row">
                <div class="col-md-4">
                    <?php $grafico->setTitulo('Quantidade por Representação')->setColors("'#55BF3B','#00BFFF', '#FFD700', '#FF6A6A'")->gerarGrafico($sqlRepresentacao); ?>
                </div>
                <div class="col-md-4">
                    <?php $grafico->setTitulo('Quantidade por Situação')->gerarGrafico($sqlSituacao); ?>
                </div>
                <div class="col-md-4">
                    <?php
                          $grafico->setTitulo('Quantidade por Resposta')
                                  ->setColors("'#55BF3B','#00BFFF', '#888', '#FFD700', '#FF6A6A'")
                                  ->gerarGrafico($sqlResposta);
                    ?>
                </div>
                <div class="col-md-12">
                    <?php
                          $grafico->setTitulo('Quantidade por Artigo')
                                  ->setTipo(Grafico::K_TIPO_COLUNA)
                                  ->setColors("'#55BF3B','#00BFFF', '#888', '#FFD700', '#FF6A6A'")
                                  ->gerarGrafico($sqlTipoItem);
                    ?>
                </div>
                <div class="col-md-12">
                    <?php
                          $grafico->setTitulo('Quantidade por Estado')
                                  ->setTipo(Grafico::K_TIPO_COLUNA)
                                  ->gerarGrafico($sqlEstados);
                    ?>
                </div>
            </div>

            <table class="table table-hover table-striped table-condensed table-bordered">
                <tr>
                    <th>Ações</th>
                    <th>UF</th>
                    <th>Município</th>
                    <th>Situação</th>
                    <th>CPF</th>
                    <th>Nome</th>
                    <th>Representação</th>
                    <th>CNPJ</th>
                    <th>Razão Social</th>
                </tr>
                <?php foreach($dados as $dado){
                    ?>
                    <tr>
                        <td><a target="_blank" href="resumoIndividual.php?queid=<?php echo $dado['queid']; ?>" title="Visualizar"><i class="glyphicon glyphicon-search"></i></a></td>
                        <td><?php echo $dado['estuf']; ?></td>
                        <td><?php echo $dado['mundescricao']; ?></td>
                        <td><?php echo $dado['situacao']; ?></td>
                        <td><?php echo $dado['parcpf']; ?></td>
                        <td><?php echo $dado['parnome']; ?></td>
                        <td><?php echo $dado['representacao']; ?></td>
                        <td><?php echo $dado['parcnpj']; ?></td>
                        <td><?php echo $dado['parreprazaosocial']; ?></td>
                    </tr>
                <?php } ?>
            </table>


        </div>

        <footer class="well well-sm">
            <p style="text-align: center;">&copy; 2015 Ministério da Educação. Todos os direitos reservados.</p>
        </footer>
        <hr>

    </div> <!-- /container -->

    <script type="text/javascript">
        $(function(){
            $('.chosen-select').chosen();
            $('#limpar_filtro').click(function(){
                window.location.href = window.location.href;
            });

            $('body').on('change', '.checkbox-uf', function() {
                console.log($('.checkbox-uf:checked').serialize());
                $('#div_municipio').load('?action=carregarMunicipio&'+$('.checkbox-uf:checked').serialize(), function() {
                    $('#muncod').chosen();
                });
            });
            $('.data').mask('99/99/9999');
        });
    </script>    
</body>
</html>