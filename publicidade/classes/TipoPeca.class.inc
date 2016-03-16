<?php

/**
 * Modelo de Tipo de Peça Publicitária
 * @author Alysson Rafael
 */
class TipoPeca extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.tipopeca';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('tppid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'tppdsc' => null
	);
	
	
	/**
     * Listagem de dados
     * @name listar_por_filtro
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return void
     */
	public function listar_por_filtro($post){
		
		$where = " WHERE tipopeca.tppstatus='A' ";
		if(!empty($post['tppdsc'])){
			$post['tppdsc'] = tirar_acentos(pg_escape_string($post['tppdsc']));
			//$where .= "AND tipopeca.tppdsc ILIKE '%{$post['tppdsc']}%' OR tipopeca.tppdsc ILIKE '%{$post['tppdsc']}%') ";
			$where .= " AND(TRANSLATE(tipopeca.tppdsc, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['tppdsc']}%' OR tipopeca.tppdsc ILIKE '%{$post['tppdsc']}%') ";
		}
		if(!empty($post['tppperiodo'])){
			$post['tppperiodo'] = pg_escape_string($post['tppperiodo']);
			$where .= "AND tipopeca.tppperiodo='{$post['tppperiodo']}' ";
		}

		
		$sql = "SELECT 
				  '<center>
                  <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||tipopeca.tppid||'\');\">
                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                  </a>
                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.tppid=tipopeca.tppid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||tipopeca.tppid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu��do!\');' END || '\">
                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                  </a>
                  </center>' as acao,
                  tipopeca.tppdsc
                  
                  FROM $this->stNomeTabela tipopeca 
                  {$where} 
                  ORDER BY tipopeca.tppdsc ";
		
		$cabecalho = array('A��o','Tipo de Pe�a Publicit�ria');
		$this->monta_lista($sql,$cabecalho,10,50,false,"center",'S','','',array('center','center'));
		
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
		if(!empty($post['tppdsc'])){
			$post['tppdsc'] = pg_escape_string(strtolower($post['tppdsc']));
			
			$sql = "SELECT tipopeca.tppid FROM $this->stNomeTabela tipopeca ";
			$sql .= "WHERE LOWER(tipopeca.tppdsc)='{$post['tppdsc']}' AND tipopeca.tppstatus='A' ";
			
			//caso seja atualização, exclui o registro atual da verificação
			if(!empty($post['tppid'])){
				$post['tppid'] = pg_escape_string($post['tppid']);
				$sql .= "AND tipopeca.tppid <> '{$post['tppid']}' ";
			}
			
			$tppid = $this->pegaUm($sql);
			
			if($tppid >= 1){
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
		
		$tppdsc = pg_escape_string($tppdsc);
		$tppperiodo = pg_escape_string($tppperiodo);
		
		$operacao = '';
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($tppid)){
			$sql = "INSERT INTO $this->stNomeTabela(tppdsc,tppperiodo)VALUES('{$tppdsc}','{$tppperiodo}') ";
		}
		else if(!empty($tppid)){
			$tppid = pg_escape_string($tppid);
			$sql = "UPDATE $this->stNomeTabela SET tppdsc='{$tppdsc}',tppperiodo='{$tppperiodo}' WHERE tppid='{$tppid}' ";
		}
		
		$sql .= " RETURNING tppid ";
		
       	$tppid = $this->pegaUm($sql);

       	if(!empty($tppid)){
       		$this->commit();
       		return array(0=>true,1=>$tppid,2=>'listaTipoPecaPublicitaria');
       	}
       	else{
       		return array(0=>false,1=>$tppid,2=>'listaTipoPecaPublicitaria');
       	}
		
	}
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $tppid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($tppid){
		if(!empty($tppid)){
			$tppid = pg_escape_string($tppid);
			$sql = "SELECT tipopeca.tppid,
						   tipopeca.tppdsc,
						   tipopeca.tppperiodo
						   FROM $this->stNomeTabela tipopeca WHERE tipopeca.tppid='{$tppid}' ";
			return $this->pegaLinha($sql);
		}
	}
	
	/**
	 * Inativa um registro
	 * @name inativar
	 * @author Alysson Rafael
	 * @access public
	 * @param int $tppid - Id do registro
	 * @return void
	 */
	public function inativar($tppid){
		
		$tppid = pg_escape_string($tppid);
		
		$sql = "UPDATE $this->stNomeTabela SET tppstatus='I' WHERE tppid='{$tppid}' ";
		$this->executar($sql);
		$this->commit();
		
		$msg = 'Exclus�o realizada com sucesso!';
		direcionar('?modulo=principal/tipopecapublicitaria/listaTipoPecaPublicitaria&acao=A',$msg);
	}
	
	/**
     * Gerar a combo de tipo de peça
     * @name monta_combo_tipo_peca
     * @author Alysson Rafael
     * @access public
     * @param int $tppid          - Qual option receberá o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @return void
     */
	public function monta_combo_tipo_peca($tppid='',$obrigatorio='N'){
		
		$sql = "SELECT tppid AS codigo,tppdsc AS descricao FROM $this->stNomeTabela
				WHERE tppstatus='A' ORDER BY tppdsc ";

		$resultado = $this->carregar($sql);

		$this->monta_combo('tppid',$resultado,'S','Selecione...','','','','',$obrigatorio,'tppid','',$tppid);

	}

	
}