<?php
/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';

// Municipio.
$_POST['itrid'] = 1;

echo $simec->title('Associar página inicial ao perfil', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">

        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Lista de Estados</h5>
                <div class="ibox-tools">
                    <a class="collapse-link"> <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content">

                <table class="table table-hover dataTable">
                    <thead>
                    <tr>
                        <th>Perfil</th>
                        <th>Página inicial</th>
                    </tr>
                    </thead>
                    <?php

                    $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil where pflstatus='A' and sisid=231 order by pfldsc ";
                    $dados = $db->carregar($sql);

                    foreach ($dados as $perfil) :

                        $sql1= "SELECT
	                            menu.mnucod as CODIGO,
	                            mnulink as DESCRICAO
	                        FROM menu
	                        LEFT JOIN perfilmenu p on menu.mnuid = p.mnuid AND p.pflcod=".$perfil['codigo']."
	                        WHERE  mnustatus = 'A' ANd menu.sisid=231
	                        ORDER BY mnucod";
                        $listaMenus = $db->carregar($sql1);
                        ?>
                        <tr>
                            <td><?php  echo $perfil['descricao']?></td>
                            <td><?php  echo $simec->select("pflcoc","","",$sql1); ?></td>
                        </tr>
                    <?php endforeach;?>
                </table>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript">
    $(document).ready( function(){
        $('#formulario').submit(function() {
// 		$("#grupo option").attr("selected", "selected").trigger('liszt:updated');
        });

    });

    function dadosUnidade( inuid ) {
        var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid;
        $(location).attr('href', url);
    }
</script>

