<?php
error_reporting(1);
error_reporting(E_ALL ^ E_NOTICE);

// set_include_path('.;D:\Workspace\php\pdeinterativo\includes;D:\Workspace\php\pdeinterativo\global;');
// $_SESSION['usucpforigem'] = '';
// $_SESSION['usucpf'] = '';
// $_SESSION['superuser'] = '1';

include "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once "classes/Encode.class.inc";
include_once "classes/Item.class.inc";
include_once "classes/Participante.class.inc";
include_once "classes/Questionario.class.inc";
include_once "classes/Avaliacao.class.inc";
include_once "classes/Comentario.class.inc";

$db = new cls_banco();

$queid = $_GET['queid'] ? $_GET['queid'] : 0;
$sql = "select
            case
            when avaresposta = 5 then 'Concordo totalmente'
            when avaresposta = 4 then 'Concordo parcialmente'
            when avaresposta = 3 then 'Não concordo e nem discordo'
            when avaresposta = 2 then 'Discordo parcialmente'
            when avaresposta = 1 then 'Discordo totalmente'
            else ''
            end as descricao,
            avaresposta, i.itedsc, comdsc,
            q.queid, p.parcpf, p.parnome, m.mundescricao, p.estuf, p.parcnpj, p.parreprazaosocial, parrepresentacao, parorgao, tpoid
        from consultapne.item i
            
            left join consultapne.avaliacao a on i.iteid = a.iteid and a.queid = $queid
            left join consultapne.comentario c on a.avaid = c.avaid
            left join consultapne.questionario q on a.queid = q.queid
            left join consultapne.participante p on q.parid = p.parid
            left join territorios.municipio m on p.muncod = m.muncod
        where i.itetipo = 'P'
        order by i.iteid, i.iteordem, avaresposta desc";
$dados  = $db->carregar($sql);
$dados = is_array($dados) ? $dados : array();
$dado = current($dados);
?>

<!DOCTYPE html>
<html lang="pt-BR">
	<?php require "head.php"; ?>

    <script src="js/jquery.min.js"></script>
    <script src="js/bootstrap.min.js"></script>
    <script src="js/jquery.chosen.js" type="text/javascript"></script>

	<body>

		<header>
			<div class="row">
				<div class="col-lg-12 col-sm-12 col-xs-12">
					<img src="imagens/logo-simec.png" class="res" width="150">
					<a class="brasil pull-right" href="http://www.brasil.gov.br/"><img alt="Brasil - Governo Federal" src="http://portal.mec.gov.br/templates/mec2014/images/brasil.png" style="margin-right: 10px;"></a>
				</div>
			</div>
		</header>

		<div class="container">

            <div class="well well-sm">
                <fieldset>
                    <legend>Informações do participante</legend>
                    <div class="row" style="padding-top: 10px;">
                        <div class="col-md-7" style="padding-right: 25px;">
                            <div class="form-group">
                                <label for="estuf">Nome: </label>
                                <?php echo $dado['parnome']; ?>
                            </div>
                        </div>
                        <div class="col-md-5" style="padding-right: 25px;">
                            <div class="form-group" id="div_municipio">
                                <label for="muncod">CPF: </label>
                                <?php echo $dado['parcpf']; ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-3" style="padding-right: 25px;">
                            <div class="form-group">
                                <label for="estuf">UF: </label>
                                <?php echo $dado['estuf']; ?>
                            </div>
                        </div>
                        <div class="col-md-4" style="padding-right: 25px;">
                            <div class="form-group" id="div_municipio">
                                <label for="muncod">Município: </label>
                                <?php echo $dado['mundescricao']; ?>
                            </div>
                        </div>
                        <div class="col-md-5">
                            <div class="form-group">
                                <label for="representacao">Tipo de Representação: </label>
                                <?php echo $dado['parrepresentacao']; ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-3" style="padding-right: 25px;">
                            <div class="form-group">
                                <label for="representacao">Tipo de Órgão: </label>
                                <?php echo $dado['tpoid']; ?>
                            </div>
                        </div>
                        <div class="col-md-9">
                            <div class="form-group ">
                                <label for="representacao">Órgão: </label>
                                <?php echo $dado['parorgao']; ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-3" style="padding-right: 25px;">
                            <div class="form-group ">
                                <label for="cnpj">CNPJ: </label>
                                <?php echo $dado['parcnpj']; ?>
                            </div>
                        </div>
                        <div class="col-md-9">
                            <div class="form-group">
                                <label for="representacao">Nome Fantasia: </label>
                                <?php echo $dado['parreprazaosocial']; ?>
                            </div>
                        </div>
                    </div>
                </fieldset>
            </div>

            <div class="row" style="padding-top: 10px;">
                <?php

                $sqlResposta = "select count(*) as valor,
                                    case
                                        when avaresposta = 5 then 'Concordo totalmente'
                                        when avaresposta = 4 then 'Concordo parcialmente'
                                        when avaresposta = 3 then 'Não concordo e nem discordo'
                                        when avaresposta = 2 then 'Discordo parcialmente'
                                        when avaresposta = 1 then 'Discordo totalmente'
                                        else 'NÃO INFORMADO'
                                    end as descricao
                                from consultapne.avaliacao a
                                    left join consultapne.questionario q on q.queid = a.queid
                                    left join consultapne.participante p on p.parid = q.parid
                                where coalesce(avaresposta, 0) != 0
                                and q.queid = $queid
                                GROUP BY descricao, avaresposta
                                order by avaresposta desc ";

                $grafico = new Grafico();
                ?>

                <div class="row">
                    <div class="col-md-12">
                        <?php
                        $grafico->setTitulo('Quantidade por Resposta')
                            ->setColors("'#55BF3B','#00BFFF', '#888', '#FFD700', '#FF6A6A'")
                            ->gerarGrafico($sqlResposta);
                        ?>
                    </div>
                </div>
            </div>

			<div class="row">
				<div class="col-lg-12 col-sm-12 col-xs-12">
                    <table class="table table-bordered table-hover table-condensed table-striped">
                        <tr>
                            <td>Artigo</td>
                            <td>Resposta</td>
                            <td>Comentário</td>
                        </tr>
                        <?php
                        $label = array(
                            1=>'label label-danger',
                            2=>'label label-warning',
                            3=>'label label-defalut',
                            4=>'label label-primary',
                            5=>'label label-success',
                        );
                        foreach ($dados as $dado) {  ?>
                            <tr>
                                <td width="10%" title="<?php echo $dado['itedsc']; ?>"><?php echo substr($dado['itedsc'], 0, 7); ?></td>
                                <td width="10%"><span class="<?php echo $label[$dado['avaresposta']]; ?>"><?php echo $dado['descricao']; ?></span></td>
                                <td><?php echo nl2br($dado['comdsc']); ?></td>
                            </tr>
                        <?php } ?>
                    </table>

				</div>
			</div>

			<hr>
		</div>
	</body>
</html>