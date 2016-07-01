<?php
/**
 * Created by PhpStorm.
 * User: RuySilva
 * Date: 05/06/14
 * Time: 19:17
 */

global $db;
?>
<div class="row">
    <div class="col-lg-12">
        <div class="well bs-component">
            <form id="formulario-pesquisar" class="form-horizontal" method="post">
                <input type="hidden" name="controller" value="instituicao">
                <input type="hidden" name="action" value="listar">
                <fieldset>
                    <legend>Pesquisar Instituições</legend>
                    <div class="form-group">
                        <label for="select" class="col-lg-2 control-label">Instituição</label>
                        <div class="col-lg-4">
                            <?php

                            //                            global $db;
                            $sqlUnidade = "SELECT
                                              inst.intid as codigo,
                                              inst.intdscrazaosocial as descricao
                                            FROM
                                              pdu.instituicao inst
                                            WHERE
                                              inst.intstatus = 'A'
                                            ORDER BY
                                              2";
                            $intid = $_REQUEST['intid'] ? $_REQUEST['entid'] : '';
                            //ver($sqlUnidade, d);

                            $intituições = (array) $db->carregar($sqlUnidade);
                            ?>

                            <select name="intid" class="form-control chosen" id="" data-placeholder="Todas">
                                <option value=""></option>
                                <?php foreach($intituições as $value): ?>
                                    <option value="<?php echo $value['codigo'] ?>"><?php echo $value['descricao'] ?></option>
                                <?php endforeach; ?>
                            </select>
                            <br>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="select" class="col-lg-2 control-label">UF</label>
                        <div class="col-lg-2">

                            <?php
                            $sqlUF = "SELECT
									estuf as codigo,
									estdescricao as descricao
								FROM
									territorios.estado
									ORDER BY 2";

                            $estuf = $_REQUEST['estuf'] ? $_REQUEST['estuf'] : '';

                            $ufs = $db->carregar($sqlUF);
                            ?>
                            <select name="estuf" class="form-control chosen" id="select" data-placeholder="Todas">
                                <option value=""></option>
                                <?php foreach($ufs as $uf): ?>
                                    <option value="<?php echo $uf['codigo'] ?>"><?php echo $uf['descricao'] ?></option>
                                <?php endforeach; ?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-10 col-lg-offset-2">
<!--                            <button type="reset" class="btn btn-default">Cancelar</button>-->
                            <button type="button" id="buscar" class="btn btn-primary">Buscar</button>
                        </div>
                    </div>
                </fieldset>
            </form>
        </div>
    </div>
    <div id="listar-pesquisa">
        <?php $this->listarAction(); ?>
    </div>
    <script>
        $('#buscar').click(function(){
            $('#formulario-pesquisar').ajaxSubmit({target: $('#listar-pesquisa').hide().fadeIn()});
        });
    </script>

