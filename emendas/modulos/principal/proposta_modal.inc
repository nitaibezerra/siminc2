<?php
$aBeneficiario = (new Emendas_Model_SiconvBeneficiario)->recuperarListagem();
$aObjetivoppa = $db->carregar($aBeneficiario);

?>

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
                        <th>Beneficiário</th>
                        <th>Id Beneficiário</th>
                      </tr>
                      </thead>
                      <tbody>
                        <?php
                         foreach($aObjetivoppa as $dados){
                          ?>
                            <tr>
                                <td class="text-center">
                                    <input type="checkbox" value="sibid">
                                </td>
                                <td><?php echo $dados['beneficiario']; ?></td>
                                <td><?php echo $dados['benid']; ?></td>


                            </tr>
                        <?php } ?>
                      </tbody>
                  </table>
              </div>
          </div>
        </div>
    </div>
</div>

<script>
    $(document).ready(function(){

        // $('.dataTablesSP').DataTable({
        //     bPaginate: false,
        //     responsive: true,
        //     dom: '<"html5buttons"B>lTfgitp',
        //     "language": {
        //         "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
        //     }
        // });
    });
</script>
