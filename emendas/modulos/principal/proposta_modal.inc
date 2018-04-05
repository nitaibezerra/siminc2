<?php
$mBeneficiario = (new Emendas_Model_SiconvBeneficiario)->recuperarListagem($_REQUEST['benid']);
$aProposta = $db->carregar($mBeneficiario);
$listaProposta = $aProposta? $aProposta:[];
?>
<form id="formulario" name="formulario" method="post" class="form-horizontal">
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
                          <th>Opçoes</th>
                          <th>CNPJ/NÚMERO da EMENDA</th>
                          <th>Beneficiario</th>
                        </tr>
                        </thead>
                        <tbody>
                          <?php
                           foreach($listaProposta as $dados){
                            ?>
                              <tr>
                                  <td class="text-center">
                                      <?php if($dados['ben'] === $dados['benid']){ ?>
                                        <input type="checkbox" name="sibid[]" value="<?php echo $dados['sibid'];?>" checked>
                                        <td><?php echo $dados['beneficiario']; ?></td>
                                        <td><?php echo $dados['benid'];?></td>
                                      <?php } else{?>
                                        <input type="checkbox" name="lista[]" value="<?php echo $dados['sibid'];?>">
                                        <td><?php echo $dados['beneficiario']; ?></td>
                                        <td><?php echo $dados['benid'];?></td>
                                      <?php }?>
                                  </td>

                              </tr>
                          <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
          </div>
      </div>
  </div>
<button class="btn btn-primary" id="#btn-salvar-proposta" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
</form>

<script type="text/javascript">
    $(function(){

        $('#btn-salvar-proposta').click(function(){
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#proposta_modal").load('?modulo=principal/emenda&acao=A&req=proposta_modal&$benid='+$('#$benid').val());
                  }
            }
            return false;
        });

    });
</script>
