<?php

$listaProposta = (new Emendas_Model_SiconvBeneficiario)->recuperarListagem($_REQUEST['benid']);

?>
<form id="formulario-selecionar-proposta" name="formulario-selecionar-proposta" method="post" class="form-horizontal">
  <input type="hidden" name="req" id="req" value="salvar-proposta" />
  <input name="benid" id="benid" type="hidden" value="<?php echo $_REQUEST['benid'];?>">

  <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                </div>
                <div class="table-responsive">
                    <table class="table table-bordered table-hover dataTablesSP" >
                        <thead>
                        <tr class="text-center">
                            <th>Opções</th>
                            <th>Nº da Proposta</th>
                            <th>Proposta</th>
                            <th>N° Situação</th>
                        </tr>
                        </thead>
                        <tbody>
                            <?php
                            foreach($listaProposta as $dados){
                               $dado = $dados['ben'] == $dados['benid']? checked:0;
                            ?>
                            <tr>
                                <td><input class="link" type="checkbox" name="sibid[]" value="<?php echo $dados['sibid'];?>" <?php echo $dado?> ></td>
                                <td class="text-center"><?php echo $dados['numeroproposta']; ?></td>
                                <td><?php echo $dados['proposta']; ?></td>
                                <td><?php echo $dados['sitdsc']; ?></td>
                            </tr>
                          <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
</form>
