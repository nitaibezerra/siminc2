<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();

        
switch ($_POST['action']) {
    
    case 'render_table':
        $sqlTable = "SELECT table_name as codigo, table_name as descricao
                     FROM information_schema.tables
                     WHERE table_schema = '{$_POST['schema']}';";
        echo $db->monta_combo("table", $sqlTable, 'S', "Selecione", '', '', '', '', 'S', 'table', '', '', '', 'required');
        exit;
        break;
    
    case 'generate':

//        bru.munhoz@bol.com.br
//20069220
        $sqlGetPrimaryKey = "SELECT  t.table_catalog,
                                        t.table_schema,
                                        t.table_name,
                                        kcu.constraint_name,
                                        kcu.column_name,
                                        kcu.ordinal_position
                               FROM    INFORMATION_SCHEMA.TABLES t
                                        LEFT JOIN INFORMATION_SCHEMA.TABLE_CONSTRAINTS tc
                                                ON tc.table_catalog = t.table_catalog
                                                AND tc.table_schema = t.table_schema
                                                AND tc.table_name = t.table_name
                                                AND tc.constraint_type = 'PRIMARY KEY'
                                        LEFT JOIN INFORMATION_SCHEMA.KEY_COLUMN_USAGE kcu
                                                ON kcu.table_catalog = tc.table_catalog
                                                AND kcu.table_schema = tc.table_schema
                                                AND kcu.table_name = tc.table_name
                                                AND kcu.constraint_name = tc.constraint_name
                               WHERE   t.table_schema NOT IN ('pg_catalog', 'information_schema')
                               AND t.table_schema = 'pes'
                               AND t.table_name = 'pesplanoacao'
                               ORDER BY t.table_catalog,
                                        t.table_schema,
                                        t.table_name,
                                        kcu.constraint_name,
                                        kcu.ordinal_position";
        
        $schema = $_POST['schema'];
        $table = $_POST['table'];
        
        $sqlGetEntity = "SELECT 
                        col.column_name, col.is_nullable, col.data_type, col.character_maximum_length 
                        , kcu.constraint_name
                        -- col.*
                        --, tab.*
                        --, tc.*
                        --, kcu.*
                FROM information_schema.columns col
                --LEFT JOIN information_schema.tables tab
                --	ON col.table_schema = tab.table_schema
                --	AND col.table_name = tab.table_name
                LEFT JOIN information_schema.table_constraints tc
                        ON tc.table_catalog = col.table_catalog
                        AND tc.table_schema = col.table_schema
                        AND tc.table_name = col.table_name
                        AND tc.constraint_type = 'PRIMARY KEY'
                LEFT JOIN information_schema.key_column_usage kcu
                        ON kcu.table_catalog = tc.table_catalog
                        AND kcu.table_schema = tc.table_schema
                        AND kcu.table_name = tc.table_name
                        --AND kcu.constraint_name = tc.constraint_name
                        AND kcu.column_name = col.column_name

                WHERE col.table_schema = '{$schema}'
                AND col.table_name = '{$table}'";
        
        $entity = $db->carregar($sqlGetEntity);
// _____________________________________________________________________________ CRIANDO MODEL INICIO
        include APPRAIZ . 'www/gerador/generatorModel.php';
        $generatorModel = new generatorModel();
        $resultModel = $generatorModel->generate($schema, $table, $entity);
// _____________________________________________________________________________ CRIANDO MODEL FIM
// _____________________________________________________________________________ CRIANDO FORM INICIO
        include APPRAIZ . 'www/gerador/generatorForm.php';
        $generatorForm = new generatorForm();
//        include APPRAIZ . "includes/cabecalho.inc";
        $resultForm = $generatorForm->generate($schema, $table, $entity);
        ver($resultForm,d);
// _____________________________________________________________________________ CRIANDO FORM FIM
        
        
        break;
    default:
        break;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

monta_titulo('Gerador de código', '&nbsp;');

?>

<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<form method="post" name="form" id="form" action="" class="formulario" >
    <input type="hidden" name="action" value="generate" />
    <table class="tabela" bgcolor="#f5f5f5" align="center">
    	<tr bgcolor="#cccccc">
            <td width="90px" style="text-align: center;" colspan="2"><strong>Formulário</strong></td>
        </tr>
<!--        <tr>
            <td width="200px" class="SubTituloDireita"><label for="dmdnome">ID:</label></td>
            <td><?php echo campo_texto('dmdid', 'N', $habilitado, 'Id da Ação', 50, 50, '', '', '', '', '', 'id="dmdid", required', ''); ?></td>
        </tr>
        <tr>
            <td width="200px" class="SubTituloDireita"><label for="dmdnome">Assunto:</label></td>
            <td><?php echo campo_texto('dmdnome', 'N', $habilitado, 'Descrição Ação', 50, 50, '', '', '', '', '', 'id="dmdnome"', ''); ?></td>
        </tr>-->
        <tr>
            <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Schema: </label></td>
            <td>
            	<?php $sqlSchema = "SELECT schema_name as codigo, schema_name as descricao  FROM information_schema.schemata"; ?>
                <?php echo $db->monta_combo("schema", $sqlSchema, 'S', "Selecione", '', '', '', '', 'S', 'schema', '', '', '', 'required'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Tabela: </label></td>
            <td id="container_selec_table">
                <?php echo $db->monta_combo("table", array(), 'S', "Selecione", '', '', '', '', 'S', 'table', '', '', '', 'required'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><label for="dmddescricao"></label></td>
            <td>
                <input id="button_save" type="button" value="Gerar"/>
            </td>
        </tr>
    </table>
</form>
<script lang="javascript">
    $('#schema').change(
            function()
            {
                url = window.location.href;
                data = {action: 'render_table', 'schema' : $(this).val()}
                $.post(url, data,function(html) {
                    $('#container_selec_table').html(html);
                });
            }
        );
        
        $('#button_save').click(function(){
            if(isValid()){
                $('#form').submit();
            }
        });
</script>
<?php

exit;









	$db = new cls_banco();
	
	$stSchema = "cte";

	$sql = "select  cl.relname as tabela,
				a.attname as coluna
			from pg_catalog.pg_attribute a
				join pg_catalog.pg_class cl on (a.attrelid = cl.oid )
				join pg_catalog.pg_namespace n on (n.oid = cl.relnamespace)
			where n.nspname = '$stSchema'
			and attstattarget != 0
			-- and relname not in ('subacaoparecertecnico')
			and relname in ( 'historicoconvitemcomposicao','historicomonitoramentoconvenio','historicomonitoramentoconvsubac','motivoitemcomposicao' )
			and cl.relkind = 'r'";

	$res = $db->carregar( $sql );
	
	foreach( $res as $arResultado ){
		$arTabelas[$arResultado["tabela"]][] = $arResultado["coluna"];
	}
	
	foreach( $arTabelas as $stTabela => $arAtributos ){
		
		if( !$arquivo = fopen( APPRAIZ."www/gerador/classes/". ucFirst($stTabela) .".class.inc", "w+" ) )
			echo "Erro ao criar o arquivo";
		
	$stClasse = '<?php
	
class '. ucFirst($stTabela) .' extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "'.$stSchema.'.'.$stTabela.'";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array('."\n";
										    
	foreach( $arAtributos as $srAtributo ){
		$stClasse .= "									  	'$srAtributo' => null, \n";
	}
	$stClasse .= "									  );
}"; 
    

	if( !fwrite( $arquivo, $stClasse ) )
		echo "Erro ao escrever no arquivo";
	else	
		echo "Classe " . $stTabela . " criada com sucesso.<br>";
		
}