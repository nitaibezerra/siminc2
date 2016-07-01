<?php

/**
 * Modelo de Contrato
 * @author Alysson Rafael
 */
class Contrato extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.contrato';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('cttid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'forid' => null
	);
	
	/**
     * Gerar a combo de status
     * @name monta_combo_status
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	public function monta_combo_status(){
		
		$situacoes = array(array('codigo'=>'A','descricao'=>'Ativo'),array('codigo'=>'I','descricao'=>'Inativo'));

		$this->monta_combo('cttstatus',$situacoes,'S','Selecione...','','','','','N','cttstatus','',$cttstatus);
		
	}
	
	
	/**
     * Verifica registro único
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['cttnumcontrato']) && !empty($post['forid'])){
			
			$post['cttnumcontrato'] = str_replace('/', '', $post['cttnumcontrato']);
			$post['cttnumcontrato'] = pg_escape_string($post['cttnumcontrato']);
			$post['forid'] = pg_escape_string($post['forid']);
			
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela ";
			$sql .= "WHERE cttnumcontrato='{$post['cttnumcontrato']}' AND forid='{$post['forid']}' ";
			if(!empty($post['cttid'])){
				$post['cttid'] = pg_escape_string($post['cttid']);
				$sql .= "AND cttid <> '{$post['cttid']}' ";
			}
            $sql .= "AND cttstatus ILIKE 'A' ";
			$qtd = $this->pegaUm($sql);
			if($qtd >= 1){
				return true;
			}
			else{
				return false;
			}
			
		}
		
	}
	
	
	/**
     * Cadastrar/editar 
     * @name salvar
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$forid = pg_escape_string($forid);
		$cttnumcontrato = str_replace('/', '', $cttnumcontrato);
		$cttnumcontrato = pg_escape_string($cttnumcontrato);
		
		
		if(!empty($cttnumsidoc)){
			$cttnumsidoc = str_replace(array('/','.','-'), '', $cttnumsidoc);
			$cttnumsidoc = pg_escape_string($cttnumsidoc);
		}
		
		$cttvigenciainicio = pg_escape_string($cttvigenciainicio);
		$cttvigenciainicio = formata_data_sql($cttvigenciainicio);
		
		$cttvigenciatermino = pg_escape_string($cttvigenciatermino);
		$cttvigenciatermino = formata_data_sql($cttvigenciatermino);
		
		$cttaditivo = pg_escape_string($cttaditivo);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($cttid)){
			$sql = "INSERT INTO $this->stNomeTabela(forid,cttnumcontrato,cttnumsidoc,cttvigenciainicio,";
			$sql .= "cttvigenciatermino,cttaditivo) ";
			$sql .= "VALUES('{$forid}','{$cttnumcontrato}','{$cttnumsidoc}',";
			$sql .= "'{$cttvigenciainicio}','{$cttvigenciatermino}','{$cttaditivo}') ";
			
		}
		else if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "UPDATE $this->stNomeTabela SET forid='{$forid}',cttnumcontrato='{$cttnumcontrato}',";
			$sql .= "cttnumsidoc='{$cttnumsidoc}',cttvigenciainicio='{$cttvigenciainicio}',";
			$sql .= "cttvigenciatermino='{$cttvigenciatermino}',cttaditivo='{$cttaditivo}' ";
			$sql .= "WHERE cttid='{$cttid}' ";
		}
		
		$sql .= "RETURNING cttid ";
		
       	$cttid = $this->pegaUm($sql);

       	if(!empty($cttid)){
       		$this->commit();
       		return array(0=>true,1=>$cttid,2=>'cadContrato');
       	}
       	else{
       		return array(0=>false,1=>$cttid,2=>'cadContrato');
       	}
		
	}
	
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "SELECT contrato.cttid,
						   contrato.forid,
						   SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
						   
						   CASE
						       WHEN CHAR_LENGTH(CAST(contrato.cttnumsidoc AS text)) >= 1 THEN 	
						           SUBSTR(CAST(contrato.cttnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 16 , 2)
						   END AS cttnumsidoc,
						   
						   
						   TO_CHAR(contrato.cttvigenciainicio,'DD/MM/YYYY') AS cttvigenciainicio,
						   TO_CHAR(contrato.cttvigenciatermino,'DD/MM/YYYY') AS cttvigenciatermino,
						   contrato.cttaditivo,
						   contrato.cttstatus
						   FROM $this->stNomeTabela contrato WHERE contrato.cttid='{$cttid}' ";
			
			return $this->pegaLinha($sql);
		}
	}
	
	/**
     * Verifica se a data final é menor que a inicial
     * @name verifica_data
     * @author Alysson Rafael
     * @access public
     * @param array $post - array com os dados
     * @return string
     */
	public function verifica_data($post){
		if(!empty($post['cttvigenciainicio']) || !empty($post['cttvigenciatermino'])){
			
			$auxInicial = explode('/',$post['cttvigenciainicio']);
			$auxFinal = explode('/',$post['cttvigenciatermino']);
			
			if(!empty($post['cttvigenciainicio'])){
				$post['cttvigenciainicio'] = strtotime(formata_data_sql($post['cttvigenciainicio']));
			}
			if(!empty($post['cttvigenciatermino'])){
				$post['cttvigenciatermino'] = strtotime(formata_data_sql($post['cttvigenciatermino']));
			}
			
			$erro = '';
			
			if(!empty($auxInicial[2]) && $auxInicial[2] < 2000){
				$erro = 'anoinicialinvalido';
			}
			else if(!empty($auxFinal[2]) && $auxFinal[2] < 2000){
				$erro = 'anofinalinvalido';
			}
			
			if(!empty($post['cttvigenciatermino']) && !empty($post['cttvigenciainicio'])){
				if($post['cttvigenciatermino'] < $post['cttvigenciainicio']){
					$erro = 'datamenor';
				}
			}
			
			
			return $erro;
		}
	}
	
	
	/**
     * Listagem de dados
     * @name listar_por_filtro
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return void
     */
	public function listar_por_filtro($post){
		
		$where = " WHERE 1=1 ";
		if(!empty($post['fornome'])){
			$post['fornome'] = tirar_acentos(pg_escape_string($post['fornome']));
			$where .= " AND(TRANSLATE(fornecedor.fornome, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['fornome']}%' OR fornecedor.fornome ILIKE '%{$post['fornome']}%') ";
		}
		if(!empty($post['cttnumcontrato'])){
			$post['cttnumcontrato'] = str_replace('/','',pg_escape_string($post['cttnumcontrato']));
			$where .= " AND contrato.cttnumcontrato = '{$post['cttnumcontrato']}' ";
		}
		if(!empty($post['cttnumsidoc'])){
			$post['cttnumsidoc'] = str_replace(array('/','.','-'),'',pg_escape_string($post['cttnumsidoc']));
			$where .= " AND contrato.cttnumsidoc = '{$post['cttnumsidoc']}' ";
		}
		if(!empty($post['cttstatus'])){
			$post['cttstatus'] = pg_escape_string($post['cttstatus']);
			$where .= " AND contrato.cttstatus = '{$post['cttstatus']}' ";
		}
		if(!empty($post['cttvigenciainicio'])){
			$post['cttvigenciainicio'] = formata_data_sql(pg_escape_string($post['cttvigenciainicio']));
			$where .= " AND contrato.cttvigenciainicio >= '{$post['cttvigenciainicio']}' ";
		}
		if(!empty($post['cttvigenciatermino'])){
			$post['cttvigenciatermino'] = formata_data_sql(pg_escape_string($post['cttvigenciatermino']));
			$where .= " AND contrato.cttvigenciatermino <= '{$post['cttvigenciatermino']}' ";
		}
        $where .= "AND contrato.cttstatus ILIKE 'A'";
		
		$anoAtual = date('Y');
//		$sql = "SELECT 
//				  
//				  CASE WHEN (SELECT COUNT(*) FROM publicidade.contratoaditivo contadi WHERE contrato.cttid=contadi.cttid) > 0 THEN
//				  	CASE WHEN (SELECT SUBSTR(CAST(contadi.ctavigenciainicio AS text) , 1 , 4) AS ano FROM publicidade.contratoaditivo contadi WHERE contrato.cttid=contadi.cttid AND contadi.ctastatus='A' ORDER BY contadi.ctaid DESC LIMIT 1) = '{$anoAtual}' THEN
//						'<center>
//	                    <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.pad pad WHERE contrato.cttid=pad.cttid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||contrato.cttid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do, pois est� relacionado a uma PAD.\');' END || '\">
//	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar contrato\">
//	                    </a>
//	                    </center>'
//				  	ELSE
//				  		'<center>
//	                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.pad pad WHERE contrato.cttid=pad.cttid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||contrato.cttid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do, pois est� relacionado a uma PAD.\');' END || '\">
//	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar contrato\">
//	                    </a>
//	                    </center>'
//	                END
//				  ELSE
//				  	CASE WHEN SUBSTR(CAST(contrato.cttvigenciainicio AS text) , 1 , 4) = '{$anoAtual}' THEN
//				  		'<center>
//	                    <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.pad pad WHERE contrato.cttid=pad.cttid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||contrato.cttid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do, pois est� relacionado a uma PAD.\');' END || '\">
//	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar contrato\">
//	                    </a>
//	                    </center>'
//				  	ELSE
//				  		'<center>
//	                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.pad pad WHERE contrato.cttid=pad.cttid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||contrato.cttid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do, pois est� relacionado a uma PAD.\');' END || '\">
//	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir contrato\">
//	                    </a>
//	                    <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||contrato.cttid||'\');\">
//	                    <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar contrato\">
//	                    </a>
//	                    </center>'
//				  	END
//				  END AS acao,
//		
//                  fornecedor.fornome,
//                  SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
//                  
//                  CASE WHEN CHAR_LENGTH(CAST(contrato.cttnumsidoc AS text)) >= 1 THEN 	
//			      	SUBSTR(CAST(contrato.cttnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 16 , 2)
//			      ELSE
//			      	''		
//			      END AS cttnumsidoc,
//			      
//			      TO_CHAR(contrato.cttvigenciainicio,'DD/MM/YYYY')||' a '||TO_CHAR(contrato.cttvigenciatermino,'DD/MM/YYYY') AS vigencia,
//			      (SELECT TO_CHAR(adi.ctavigenciainicio,'DD/MM/YYYY')||' a '||TO_CHAR(adi.ctavigenciatermino,'DD/MM/YYYY') FROM publicidade.contratoaditivo adi WHERE contrato.cttid=adi.cttid ORDER BY adi.ctaid DESC LIMIT 1) AS aditivo
//			      
//                  
//                  FROM $this->stNomeTabela contrato 
//                  JOIN publicidade.fornecedor fornecedor ON contrato.forid=fornecedor.forid
//                  {$where} 
//                  ORDER BY contrato.cttnumcontrato,fornecedor.fornome ";

                $sql = "SELECT 
'<center>
	                    <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||contrato.cttid||'\');\">
	                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar contrato\">
	                    </a>
	                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.pad pad WHERE contrato.cttid=pad.cttid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||contrato.cttid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do, pois est� relacionado a uma PAD.\');' END || '\">
	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir contrato\">
	                    </a>
	                    <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||contrato.cttid||'\');\">
	                    <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar contrato\">
	                    </a>
	                    </center>' AS acao,
		
                  fornecedor.fornome,
                  SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
                  
                  CASE WHEN CHAR_LENGTH(CAST(contrato.cttnumsidoc AS text)) >= 1 THEN 	
			      	SUBSTR(CAST(contrato.cttnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(contrato.cttnumsidoc AS text) , 16 , 2)
			      ELSE
			      	''		
			      END AS cttnumsidoc,
			      
			      TO_CHAR(contrato.cttvigenciainicio,'DD/MM/YYYY')||' a '||TO_CHAR(contrato.cttvigenciatermino,'DD/MM/YYYY') AS vigencia,
			      (SELECT TO_CHAR(adi.ctavigenciainicio,'DD/MM/YYYY')||' a '||TO_CHAR(adi.ctavigenciatermino,'DD/MM/YYYY') FROM publicidade.contratoaditivo adi WHERE contrato.cttid=adi.cttid ORDER BY adi.ctaid DESC LIMIT 1) AS aditivo
			      
                  
                  FROM $this->stNomeTabela contrato 
                  JOIN publicidade.fornecedor fornecedor ON contrato.forid=fornecedor.forid
                  {$where} 
                  ORDER BY contrato.cttnumcontrato,fornecedor.fornome ";
                  
//                  ver($sql,d);
		$cabecalho = array('A&ccedil;&atilde;o','Ag&ecirc;ncia','N&#186; Contrato','N&#186; Sidoc','Per&iacute;odo de Vig&ecirc;ncia','&Uacute;ltimo aditivo');
		$param['ordena'] = false;
		$param['totalLinhas'] = true;
		$this->monta_lista($sql,$cabecalho,10,50,false,"center",'','','','','',$param);
		
	}
	
	/**
     * Inativa registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return void
     */
	public function inativar($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "UPDATE $this->stNomeTabela SET cttstatus='I' WHERE cttid='{$cttid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	
	/**
     * Pega contrato pela agência(fornecedor)
     * @name pegar_registro_por_agencia
     * @author Alysson Rafael
     * @access public
     * @param int $forid - Identificador do registro pai
     * @return array
     */
	public function pegar_registro_por_agencia($forid){
		if(!empty($forid)){
			$forid = pg_escape_string($forid);
			$data = date('Y-m-d');
			$sql = "SELECT cttid,
			               SUBSTR(CAST(cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato
			               FROM $this->stNomeTabela 
			               WHERE forid='{$forid}' and cttstatus = 'A'  
						  
						   ORDER BY cttid DESC LIMIT 1 ";
			// AND cttvigenciainicio <= '{$data}' AND cttvigenciatermino >= '{$data}'
			$resultado = $this->carregar($sql);
			return $resultado[0];
		}
	}
	
	/**
     * Pega o valor total do orçamento do contrato
     * @name pegar_total_valor_orcamento
     * @author Alysson Rafael
     * @access public
     * @param string $cttidsStr - Ids dos contratos
     * @return float
     */
	public function pegar_total_valor_orcamento($cttidsStr,$ano){

      		if(!empty($cttidsStr)){
			$sql = "SELECT SUM(cto.ctovalor)
					FROM publicidade.contratoorcamento cto
					WHERE cto.cttid IN({$cttidsStr}) AND cto.ctostatus='A'
                                        AND ctoanoexercicio = $ano";

			return $this->pegaUm($sql);
		}
	}

	
}