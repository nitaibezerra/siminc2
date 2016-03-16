<?php
// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT
			t.table_schema AS esquema,
			t.table_name AS tabela,
			c.column_name AS coluna,
			c.data_type AS tipo,
			c.character_maximum_length AS max_length,
			c.is_nullable AS nulo, 
			c.column_default AS default,
			CASE WHEN tc.constraint_type = 'PRIMARY KEY'
				THEN 'PK'
			WHEN tc.constraint_type = 'FOREIGN KEY'
				THEN 'FK'
				ELSE ''
			END AS tipo_constraint
		FROM
			information_schema.tables t
		INNER JOIN 
			information_schema.columns c ON c.table_schema = t.table_schema
											AND c.table_name = t.table_name
		left JOIN  
			information_schema.key_column_usage pk ON pk.table_schema = t.table_schema
											          AND pk.table_name = t.table_name
											          AND pk.column_name = c.column_name				
		left JOIN 
			information_schema.table_constraints tc ON tc.table_schema = t.table_schema
													   AND tc.table_name = t.table_name			
												       AND tc.constraint_name = pk.constraint_name	
		WHERE
			t.table_schema = 'assint'
			AND t.table_name = 'programafontefin'";
//.
$dados = $db->carregar($sql);

$schema = $dados[0]['esquema'];
$tabela = $dados[0]['tabela'];

$cod = '
/*
 * OBSERVAÇÃO IMPORTANTE
 * A classe onde você inserir o método "manter' . ucfirst($tabela) . '", gerado pelo script,
 * deve conter este código. 

	protected $db;
	
	function __construct(){
		include(APPRAIZ. \'includes/classes/DBMontagemValidacao.inc\');
		include(APPRAIZ. \'includes/classes/DBComando.inc\');
		$this->db = new DBComando();
	}

*/

/*
 * Função  manter' . ucfirst($tabela) . '
 * Método usado para manter (insert/update) os dados da tabela (' . $schema . '.' . $tabela . ') 
 * 
 * @access   public
 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
 * @since    ' . date('d-m-Y') . '
 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
 * @tutorial Array(
			[evento] => manter
			[benid] => 26
			[prgid] => 40
			[bendtinicial] => 14/10/2009
			[bendtfinal] => 15/10/2009
			[btalterar] => Salvar
		)
 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE). 
 * @tutorial Array(
			[evento] => manter
			[benid] => 26
			[prgid] => 40
			[bendtinicial] => 14/10/2009
			[bendtfinal] => 15/10/2009
			[btalterar] => Salvar
		)
 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
 */
function manter' . ucfirst($tabela) . '($dados, $where = null){
	$return   = true;
	$tabela   = "' . $schema . '.' . $tabela . '";	

	// Mapeamento dos campos da tabela
	$atributo = (Object) array(';

	foreach ($dados as $val){
		$nulo 			= ($val['nulo'] == 'NO' ? 'false' : 'true' );
		$val['default'] = ((strlen($val['default']) < 20 && !is_null($val['default'])) ? '"' . $val['default'] . '"' : 'null');
		
		if (strpos($val['tipo'], 'character') > -1){
			$tipo = 'string';
			if (strpos($val['coluna'], 'cpf') > -1){
				$mascara = 'cpf';
				$val['default'] = $val['default'] == 'null' ? '$_SESSION[\'usucpf\']' : $val['default'];
			}
			
			if (strpos($val['coluna'], 'status') > -1 && $val['default'] == 'null'){
				$val['default'] = "'A'";
			}	
			
		}elseif (strpos($val['tipo'], 'integer') > -1){
			$tipo = 'integer';				
		}elseif (strpos($val['tipo'], 'timestamp') > -1 || strpos($val['tipo'], 'date') > -1){
			$tipo 	 = 'data';				
			$mascara = 'data';	
			if ($nulo == 'false'){			
				$val['default'] = 'date(\'d-m-Y\')';
			}
		}else{
			$tipo = $val['tipo'];
		}
		
		$cod .= '
				"' . $val['coluna'] . '" => array(
						"chave"   => ' . ($val['tipo_constraint'] ? '"' . $val['tipo_constraint'] . '"' : 'null') . ',		
						"value"   => ' . $val['default'] . ',
						"type"    => "' . $tipo . '",
						"tamanho" => ' . ($val['max_length'] ? '"' . $val['max_length'] . '"' : 'null') . ',
						"mascara" => ' . ($mascara ? '"' . $mascara . '"' : 'null') . ',
						"nulo"    => ' . $nulo . ',
					),';		
		unset($tipo, $mascara);	
	}	
$cod .= '		
			);
			
	if (is_array($where) && !empty($where)){
		// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
		//$atributoWhere = clone $atributo;
		
		// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo 
		foreach ($where as $k => $val){
			if (isset($atributo->{$k})){
				$atributoWhere->{$k}[\'value\'] = $val;
			}
		}
	}else{
		$atributoWhere = null;
	}
							  
	if (is_array($dados)  && !empty($dados)){
		// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
		foreach ($dados as $k => $val){
			if (isset($atributo->{$k})){
				$atributoUpdate->{$k} 		     = $atributo->{$k};
				$atributo->{$k}[\'value\'] 	     = $val;
				$atributoUpdate->{$k}[\'value\'] = $val;
			}
		}
		// Caso seja update, desconsidera os valores padrões
		if (!is_null($atributoWhere)){
			$atributo = $atributoUpdate;
		}		
	// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO	
	}else{
		return false;
	}
													 
	// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
	// retornará FALSE
	// senão o ID do insert
	$return = $this->db->insert($tabela, $atributo, $atributoWhere);
	
	// Verificação do retorno
	// Este IF só deve ser usado no código, quando for a última operação de banco  
	if ($return){
		$this->db->commit();
	}else{
		$this->db->rollback();		
	}
	
	return $return;
}
';

dbg($cod, 1);














//ucfirst($foo); 
dbg($dados);
?>