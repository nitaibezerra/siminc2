<?php
/**
 * Popup de criação de questionários
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 * @version $Id$
 */
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen">
<script type="text/javascript">
    $(function(){
         setTimeout(function() {
             $("#subacoes").chosen({no_results_text: "Oops, Nada encontrado!"}); 
             $("#id-percod").chosen({no_results_text: "Oops, Nada encontrado!"});
         },500);
         
    });
    
    function inserirQuestionario() {
        if (jQuery('#qstnome').val() == '') {
            alert('Nome do questionário em branco');
            return false;
        }
        if (jQuery('#id-percod').val() == '') {
            alert('Período em branco');
            return false;
        }
        selectAllOptions(document.getElementById('id_subacoes'));
        document.getElementById('formquestionario').submit();    
        $('#formquestionario').submit();
    }
</script>
<form class="form-horizontal" method="post" name="formquestionario" id="formquestionario">
    <input type="hidden" name="requisicao" value="inserirQuestionario">
    <section class="form-group">
        <label class="control-label col-md-2" for="nome">Nome:</label>
        <section class="col-md-10">
            <?php inputTexto('qstnome', $valor, 'nome', 150,false); ?>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="subacoes">Subações:</label>
        <section class="col-md-10">            
        <?
        $sql = "
            SELECT 
                s.id_subacao AS codigo,
                s.codigo || ' - ' || titulo AS descricao
            FROM planacomorc.subacao s
            WHERE id_exercicio ='" . $_SESSION ['exercicio'] . "' 
        ";                        
        inputCombo ( 'id_subacoes[]', $sql, $valor, 'subacoes', array ('multiple' => 'multiple' ,'titulo'=>'Selecione'));
        ?>
            
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="percod">Períodos:</label>
        <section class="col-md-10">
        <?
        $sql = "
            SELECT 
                id_periodo_referencia as codigo,
                titulo || ' (' || to_char(inicio_validade,'MM/YYYY') ||' até ' || to_char(fim_validade,'MM/YYYY') || ') / ' || id_exercicio as descricao
            FROM planacomorc.periodo_referencia
            WHERE id_exercicio = {$_SESSION['exercicio']}
            ORDER BY 1
        ";        
        inputCombo ( 'percod', $sql, $valor, 'id-percod' );
        ?>
        </section>
    </section>	
</form>
<?php
exit();