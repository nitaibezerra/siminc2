<?php
/**
 * Arquivo de criação do formulário de carga de Extração de Custeio da Folha de Pagamento
 * $Id: formCarga.inc 78720 2014-04-10 21:27:01Z maykelbraz $
 */
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
    $('documento').ready(function () {
        // $('#buttonSend').click(function() {
        // 	$('#cargaLiberacaoFinanceira').submit();
        // });
        $('input[type=file]').bootstrapFileInput();

        $('#processar').click(function () {
            $('#requisicao').val('processarDados');
            $('#cargaLiberacaoFinanceira').submit();
        });

        $('#buttonClear').click(function () {
            url = 'progfin.php?modulo=principal/custeiofolhapagamento/carga&acao=A';
            $(location).attr('href', url);
        });
    });

    function trocaMesCarga(mes) {
        location.href = "progfin.php?modulo=principal/custeiofolhapagamento/carga&acao=A&mes=" + mes;
    }

    //function no botão Gerar excel, passando parametros para carregar no arquivo carga.inc
    function geraExcel() {
        window.document.getElementById('reqexcel').value = 'excel';
        // $('#cargaLiberacaoFinanceira').attr('target', '_blank');
        //$('#requisicao').val("");//primeira verificacao é se é excel, não precisa limpar.
        $('#mes_2').val($('#mes').val()); //recuperando o parâmetro do mes da tela carga.
        document.cargaLiberacaoFinanceira.submit();
        window.document.getElementById('reqexcel').value = '';
    }
</script>
<div class="well col-md-12">
    <form name="cargaLiberacaoFinanceira" id="cargaLiberacaoFinanceira" enctype="multipart/form-data" method="POST">
        <input type="hidden" name="requisicao" id="requisicao" value="processarArquivo" />
        <input type="hidden" name="reqexcel" id="reqexcel" value=""/>
        <input type="hidden" name="mes" id="mes_2" value="" />

        <div class="form-group">
            <div class="col-md-10">
                <input type="file" title="Carregar arquivo de custeio da folha de pagamento (.txt)"
                       name="custeiofolhapagamento" class="btn btn-primary start" />
                <input type="hidden" name="action" id="action" value="carregar" />
            </div>
        </div>
        <br style="clear: both"/>
        <br style="clear: both"/>
        <label for="trocaMesCarga" >Mês:</label>
        <div >

            <?php
            $sql = "  SELECT 
                    DISTINCT cfpmes as codigo,
                    CASE WHEN cfpmes::integer = 1 THEN 'Janeiro' 
                    WHEN cfpmes::integer = 2 THEN 'Fevereiro'
                    WHEN cfpmes::integer = 3 then 'Março'
                    WHEN cfpmes::integer = 4 THEN 'Abril'
                    WHEN cfpmes::integer = 5 THEN 'Maio'
                    WHEN cfpmes::integer = 6 THEN 'Junho'
                    WHEN cfpmes::integer = 7 THEN 'Julho'
                    WHEN cfpmes::integer = 8 THEN 'Agosto'
                    WHEN cfpmes::integer = 9 THEN 'Setembro'
                    WHEN cfpmes::integer = 10 THEN 'Outubro'
                    WHEN cfpmes::integer = 11 THEN 'Novembro'
                    WHEN cfpmes::integer = 12 THEN 'Dezembro' end as descricao
              FROM progfin.cargacusteiofolhapagamento
              WHERE cfpano::integer =  {$_SESSION['exercicio']}";
            if (isset($_REQUEST ['mes']) && $_REQUEST ['mes'] != '') {
                $mes = $_REQUEST ['mes'];
            } else {
                $mes = $db->pegaUm("SELECT MAX(cfpmes::integer) FROM progfin.cargacusteiofolhapagamento WHERE cfpano::integer =  {$_SESSION['exercicio']} ");
            }
            inputCombo('mes', $sql, $mes, 'trocaMesCarga');
            ?>
        </div>
</div>
<button onclick="divCarregando();" class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
<button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
<input class="btn btn-primary start" type="button" name="" value="Gerar XLS" onclick="return geraExcel();"/>
<button class="btn btn-danger" id="processar" type="button"><i class="glyphicon glyphicon-export"></i> Processar Mês Selecionado</button>
<br style="clear:both" />
</form>
<br style="clear:both" />
<?php
$where = "";
/* Elementos de Despesa  */
$sql = "SELECT
                rccelementodespesa
            FROM
                progfin.regrascargacusteiofolhapagamento";
foreach ($db->carregar($sql) as $value) {
    $elementos .= $value['rccelementodespesa'] . ",";
}
$elementos = substr($elementos, 0, -1);
/* Rubricas  */
$sql = "SELECT
                rccrubrica
            FROM
                progfin.regrascargacusteiofolhapagamento";
foreach ($db->carregar($sql) as $value) {
    $rubricas .= $value['rccrubrica'] . ",";
}
$rubricas = substr($rubricas, 0, -1);
$sql = "
        SELECT DISTINCT 
            cfpcodigodespesa ,
            cfprubrica
       FROM
        progfin.cargacusteiofolhapagamento
       WHERE
        cfpmes::integer = {$mes} AND cfpcodigodespesa::INTEGER NOT IN ($elementos)  AND cfprubrica::INTEGER NOT IN(0,$rubricas)";

#ver($sql,d);
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
$listagem->setQuery($sql)
        ->setCabecalho(array('Elemento Despesa', 'Rubrica'));
echo montaItemAccordion(
        '<span class="glyphicon glyphicon-info-sign"></span> Elementos de Despesa / Rubricas sem Regra(s) para o mês selecionado', 'resumocat', $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM), array('accordionID' => 'accordion2', 'retorno' => true)
);
require(dirname(__FILE__) . "/listarCarga.inc");
?>
</div>