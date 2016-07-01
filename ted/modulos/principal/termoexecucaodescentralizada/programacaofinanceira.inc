<?php
//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();

Ted_Utils_Model::userIsAllowed();

//Cabeçalho do sistema
include  APPRAIZ . 'includes/cabecalho.inc';

?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Programação Financeira</li>
    </ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

    <section class="col-md-10">
        <form class="form-horizontal" role="form">
            <div class="form-group">
                <label class="control-label col-md-2" for="solicitar">Solicitar P.F.</label>
                <div class="col-md-4">
                    <input type="checkbox" id="solicitar" value="1" data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini" />
                </div>
            </div>

            <div class="form-group">
                <div class="col-md-offset-2">
                    <button type="submit" class="btn btn-primary" name="search" id="search">Salvar</button>
                </div>
            </div>
        </form>

        <br><br><br>
        <h3 class="text-center">Programação Financeira Realizadas para esse TED:</h3>

        <table class="table">
            <tr>
                <th>Pedido / Lote</th>
                <th>Criação</td>
                <th>Unidade Gestora (UG)</th>
                <th>Fonte</th>
                <th>PF</th>
            </tr>
            <tr>
                <td class="text-center info" colspan="5">
                    <p>Nenhum registro encontrado.</p>
                </td>
            </tr>
            <!--
            <tr>
                <td class="text-center">0004920</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            <tr>
                <td class="text-center">0004912</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            <tr>
                <td class="text-center">0004912</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            <tr>
                <td class="text-center">0004914</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            <tr>
                <td class="text-center">0004918</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            <tr>
                <td class="text-center">0004915</td>
                <td class="text-center">04/12/2014</td>
                <td>150028 - Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</td>
                <td class="text-center">0113150072</td>
                <td class="text-center">-</td>
            </tr>
            -->
        </table>
    </section>
</section>
