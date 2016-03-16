<?php 
    $dados = $_REQUEST; 
    global $db;
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >

<script>    
    $(function(){
         setTimeout(function() { $("#subacoes").chosen({no_results_text: "Oops, Nada encontrado!"}); },500);        
    });
    
    function inserirProduto() {
	if(jQuery('#codigo').val()=='') {
		alert('Código do produto em branco');
		return false;
	}

        if(jQuery('#nome').val()=='') {
                alert('Nome do produto em branco');
                return false;
        }

        if(jQuery('#descricao').val()=='') {
                alert('Descrição em branco');
                return false;
        }

        document.getElementById('formproduto').submit();
        //jQuery('#formproduto').submit();
    }
</script>

    <form class="form-horizontal" method="post" name="formproduto" id="formproduto">
        <?php 
        if($dados['id_produto_spo']) {
            $sql = "SELECT id_produto_spo, codigo, nome, descricao FROM planacomorc.produto_spo WHERE id_produto_spo='".$dados['id_produto_spo']."'";
            $produtosubacao = $db->pegaLinha($sql);

            echo '<input type="hidden" name="requisicao" value="atualizarProduto">';
            echo '<input type="hidden" name="id_produto_spo" value="'.$produtosubacao['id_produto_spo'].'">';

            global $id_subacao;

            $sql = "
                SELECT 
                    s.id_subacao as codigo, 
                    s.codigo || ' - ' || s.descricao as descricao
                FROM planacomorc.subacao s
                INNER JOIN planacomorc.produto_spo_subacao p ON p.id_subacao = s.id_subacao
                WHERE p.id_produto_spo='".$dados['id_produto_spo']."'";
            //adicionando ano de exercicio, para carregar combo popup de subações correspondentes.
            
            $selectedItems = array ();
            if ($id_subacao = $db->carregar($sql)) {
                foreach ($id_subacao as $item) {                    
                    $selectedItems[] = $item['codigo'];                    
                }
            }
        } else {
            echo '<input type="hidden" name="requisicao" value="inserirProduto">';
        }        
        ?>

        <section class="form-group">
            <label class="col-md-2 control-label" for="id_exercicio">Ano Exercício:</label>
            <section class="col-md-10">
            <?php //echo campo_texto('id_exercicio', "N", "N", "Ano Exercício", 4, 4, "", "", '', '', 0, 'id="id_exercicio"', '',  );?>
            <?php inputTexto('id_exercicio', $_SESSION['exercicio'], 'id_exercicio', 4,false,array('habil' => 'N', 'obrig' => 'N'));?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="codigo">Código:</label>
            <section class="col-md-10">
            <?php //echo campo_texto('codigo', "N", "S", "Nome", 12, 10, "", "", '', '', 0, 'id="codigo"', '', $produtosubacao['codigo'] );?>
            <?php inputTexto('codigo', $produtosubacao['codigo'], 'codigo', 10,false,array('obrig'=>'N'))?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="nome">Nome:</label>
            <section class="col-md-10">
                    <?php //echo campo_texto('nome', "S", "S", "Nome", 62, 100, "", "", '', '', 0, 'id="nome"', '',  );?>
                    <?php inputTexto('nome', $produtosubacao['nome'], 'nome', 100,false);?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="descricao">Descrição:</label>
            <section class="col-md-10">
            <?php //echo campo_textarea( 'descricao', 'S', 'S', '', '70', '4', '500', '', '', '', '', '', $produtosubacao['descricao']);?>
            <?php inputTextArea('descricao', $produtosubacao['descricao'], 'descricao', 500,array('obrig'=>'S'));?>
            </section>
        </section>

        <section class="form-group">
            <label class="col-md-2 control-label" for="subacoes">Subações:</label>
            <section class="col-md-10">
            <?php 
            $sql = "
                SELECT 
                    id_subacao as codigo, 
                    codigo || ' - ' || descricao as descricao
                FROM planacomorc.subacao s
                WHERE id_exercicio='".$_SESSION['exercicio']."'
                    AND EXISTS (
                    SELECT 1
                    FROM planacomorc.snapshot_dotacao_subacao sds
                    WHERE s.id_subacao = sds.id_subacao)
            ";    	
            
            //combo_popup( "id_subacao", $sql, "Ações", "400x600", 0, array(), "", "S", false, false, 5, 400 );
            inputCombo('id_subacao[]', $sql, $selectedItems, 'subacoes',array('multiple'=>'multiple','titulo'=>'Selecione'));   			    			
            ?>
            </section>
        </section>    	       
    </form>	

<?php exit();?>