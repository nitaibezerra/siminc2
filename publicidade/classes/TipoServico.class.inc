<?php

/**
 * Modelo de Tipo de ServiÃ§o
 * @author Alysson Rafael
 */
class TipoServico extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.tiposervico';

	/**
	 * Chave primÃ¡ria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('tseid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'tsedsc' => null
	);
	
	/**
     * Gerar a combo de tipo de serviÃ§o
     * @name monta_combo_tipo_servico
     * @author Alysson Rafael
     * @access public
     * @param int $tseid          - Qual option receberÃ¡ o selected
     * @param string $obrigatorio - Se a combo vai ou nÃ£o exibir a imagem de obrigatÃ³rio(S ou N)
     * @param string $origem      - De onde veio a chamada
     * @return void
     */
	public function monta_combo_tipo_servico($tseid='',$obrigatorio='N',$origem=''){
		
		$sql = "SELECT tseid AS codigo,tsedsc AS descricao FROM $this->stNomeTabela ORDER BY tsedsc ";

		$resultado = $this->carregar($sql);
		if(!$resultado){
			$resultado = array();
		}
		
		if($origem == 'cadastro' || $origem == 'listaItens'){
			$this->monta_combo('tseid',$resultado,'S','Selecione...','carregarCategoria(this.value,\'\',\''.$obrigatorio.'\');pegarDscTipoServico(this.value,\'successPegarDscTipoServico\',\'failurePegarDscTipoServico\',\'createPegarDscTipoServico\',\'completePegarDscTipoServico\');verificarHonorarioTipoServico(this.value);','','','',$obrigatorio,'tseid','',$tseid);
		}
		else{
			$this->monta_combo('tseid',$resultado,'S','Selecione...','','','','',$obrigatorio,'tseid','',$tseid);
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
		
		$where = " WHERE cat.catstatus='A' ";
		if(!empty($post['tseid'])){
			$post['tseid'] = pg_escape_string($post['tseid']);
			$where .= "AND tiposervico.tseid = '{$post['tseid']}' ";
		}
//		if(!empty($post['catdsc'])){
//			$post['catdsc'] = tirar_acentos(pg_escape_string($post['catdsc']));
//			$where .= " AND(TRANSLATE(cat.catdsc, 'Ã¡Ã©Ã­Ã³ÃºÃ Ã¨Ã¬Ã²Ã¹Ã£ÃµÃ¢ÃªÃ®Ã´Ã´Ã¤Ã«Ã¯Ã¶Ã¼Ã§ÃÃ‰ÃÃ“ÃšÃ€ÃˆÃŒÃ’Ã™ÃƒÃ•Ã‚ÃŠÃŽÃ”Ã›Ã„Ã‹ÃÃ–ÃœÃ‡', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['catdsc']}%' OR cat.catdsc ILIKE '%{$post['catdsc']}%') ";
//			//$where .= " AND cat.catdsc ILIKE '%{$post['catdsc']}%' OR cat.catdsc ILIKE '%{$post['catdsc']}%') ";
//		}
                if(!empty($post['catdsc'])){
			$where .= " AND catdsc ilike '%{$post['catdsc']}%'";
		}

		
		$sql = "SELECT 
				  '<center>
                  <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||cat.catid||'\');\">
                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                  </a>
                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.catid=cat.catid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||cat.catid||'\');}' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                  </a>
                  </center>' as acao,
                  tiposervico.tsedsc,
                  cat.catdsc||' ' 
                  
                  FROM $this->stNomeTabela tiposervico
                  JOIN publicidade.categoria cat ON tiposervico.tseid=cat.tseid 
                  {$where} 
                  ORDER BY cat.catdsc,tiposervico.tsedsc ";
		
		$cabecalho = array('Ação','Tipo de Serviço','Categoria');
		$this->monta_lista($sql,$cabecalho,10,50,false,"center",'S','','',array('center','center','center'));
		
	}
	
	/**
     * Verifica registro Ãºnico
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['tseid'])){
			$post['tseid'] = pg_escape_string($post['tseid']);
			$post['catdsc'] = pg_escape_string($this->strtolower_utf8($post['catdsc']));
			
			$sql = "SELECT tiposervico.tseid FROM $this->stNomeTabela tiposervico ";
			$sql .= "JOIN publicidade.categoria cat ON tiposervico.tseid=cat.tseid ";
			$sql .= "WHERE LOWER(cat.catdsc)='{$post['catdsc']}' AND cat.tseid='{$post['tseid']}' ";
			//$sql .= "AND cat.catstatus='A' AND tiposervico.tsestatus='A' ";
			$sql .= "AND tiposervico.tsestatus='A' ";
			
			//caso seja atualizaÃ§Ã£o, exclui o registro atual da verificaÃ§Ã£o
			if(!empty($post['catid'])){
				$post['catid'] = pg_escape_string($post['catid']);
				$sql .= "AND cat.catid <> '{$post['catid']}' ";
			}
			
			$tseid = $this->pegaUm($sql);
			
			if($tseid >= 1){
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
     * @param array $post - Dados do formulÃ¡rio
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$tseid = pg_escape_string($tseid);
		$catdsc = pg_escape_string($catdsc);
		
		$operacao = '';
		
		//caso o id esteja vazio, a operaÃ§Ã£o serÃ¡ de insert
		//caso nÃ£o esteja vazio, a operaÃ§Ã£o serÃ¡ de update
		if(empty($catid)){
			$sql = "INSERT INTO publicidade.categoria(tseid,catdsc)VALUES('{$tseid}','{$catdsc}') ";
		}
		else if(!empty($catid)){
			$catid = pg_escape_string($catid);
			$sql = "UPDATE publicidade.categoria SET tseid='{$tseid}',catdsc='{$catdsc}' ";
			$sql .= "WHERE catid='{$catid}' ";
		}
		
		$sql .= " RETURNING catid ";
		
       	$catid = $this->pegaUm($sql);

       	if(!empty($catid)){
       		$this->commit();
       		return array(0=>true,1=>$catid,2=>'listaTipoServicoCategoria');
       	}
       	else{
       		return array(0=>false,1=>$catid,2=>'listaTipoServicoCategoria');
       	}
		
	}
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $catid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($catid){
		if(!empty($catid)){
			$catid = pg_escape_string($catid);
			$sql = "SELECT cat.catid,
						   cat.tseid,
						   cat.catdsc
						   FROM $this->stNomeTabela tiposervico 
						   JOIN publicidade.categoria cat ON tiposervico.tseid=cat.tseid 
						   WHERE cat.catid='{$catid}' ";
			return $this->pegaLinha($sql);
		}
	}
	
	/**
	 * Inativa um registro
	 * @name inativar
	 * @author Alysson Rafael
	 * @access public
	 * @param int $catid - Id do registro
	 * @return void
	 */
	public function inativar($catid){
		
		$catid = pg_escape_string($catid);
		
		$sql = "UPDATE publicidade.categoria SET catstatus='I' WHERE catid='{$catid}' ";
		$this->executar($sql);
		$this->commit();
		
		direcionar('?modulo=principal/tiposervicocategoria/listaTipoServicoCategoria&acao=A','Exclusão realizada com sucesso!');
	}
	
	/**
	 * Recupera os registros
	 * @name carrega_registros
	 * @author Alysson Rafael
	 * @access public
	 * @return array
	 */
	public function carrega_registros(){
		$sql = "SELECT tseid,tsedsc FROM $this->stNomeTabela ORDER BY tsedsc ";
		$resultado = $this->carregar($sql);
		return $resultado;
	}
	
	/**
     * Gerar a combo de categoria
     * @name monta_combo_categoria
     * @author Alysson Rafael
     * @access public
     * @param int $tseid          - Identificador de tipo de serviÃ§o
     * @param int $catid          - Identificador de categoria
     * @param string $obrigatorio - Se a combo vai ou nÃ£o exibir a imagem de obrigatÃ³rio(S ou N)
     * @return void
     */
	public function monta_combo_categoria($tseid,$catid='',$obrigatorio='N'){
		
		if(!empty($tseid)){

			$tseid = pg_escape_string($tseid);
			
			$where = "WHERE tseid={$tseid} ";
		
			$sql = "SELECT catid AS codigo,catdsc AS descricao FROM publicidade.categoria {$where} AND catstatus = 'A' ORDER BY catdsc ";
			$resultado = $this->carregar($sql);
                        //teste usuario;
		//ver($sql);
		}
		else{
			$resultado = array();
                         //teste usuario;
               // ver($resultado);
		}
		$this->monta_combo('catid',$resultado,'S','Selecione...','','','','',$obrigatorio,'catid','',$catid);
	}
	
	/**
     * Pega a descriÃ§Ã£o
     * @name pegar_descricao
     * @author Alysson Rafael
     * @access public
     * @param int $tseid - Identificador do registro
     * @return string
     */
	public function pegar_descricao($tseid){
		if(!empty($tseid)){
			$tseid = pg_escape_string($tseid);
			$sql = "SELECT tsedsc FROM $this->stNomeTabela WHERE tseid='{$tseid}' ";
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	/**
     * Pega os tipos de serviÃ§o e as categorias para o relatÃ³rio
     * @name pegar_tipos_prestacao_contas
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
	public function pegar_tipos_prestacao_contas($get){
		if(!empty($get['dtinicial']) && !empty($get['dtfinal'])){
			$get['dtinicial'] = pg_escape_string(formata_data_sql($get['dtinicial']));
			$get['dtfinal'] = pg_escape_string(formata_data_sql($get['dtfinal']));
			$get['agencias'] = pg_escape_string($get['agencias']);
		
			$sql = "SELECT 
					tse.tsedsc AS tsedsc,
					cat.catdsc AS catdsc,
					SUM(CAST(pgto.pipvalorfaturado AS numeric)) AS valor
					FROM publicidade.tiposervico tse
					JOIN publicidade.categoria cat ON tse.tseid=cat.tseid
					JOIN publicidade.itenspad item ON tse.tseid=item.tseid AND cat.catid=item.catid
					JOIN publicidade.pad pad ON item.padid=pad.padid
					JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
					JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
					JOIN publicidade.fornecedor fornec ON cont.forid=fornec.forid
					WHERE(pgto.pipdatapagamento >= '{$get['dtinicial']}' AND pgto.pipdatapagamento <= '{$get['dtfinal']}' AND pgto.pipstatus = 'A') 
					AND tse.tsestatus='A'
					AND cat.catstatus='A'
					AND item.ipastatus='P'
					AND pad.padstatus='A'
					AND cont.cttstatus='A'
					AND fornec.forstatus='A'
				    AND fornec.foragencia='1'
				    AND fornec.forid IN ({$get['agencias']})
					
					GROUP BY tse.tsedsc,cat.catdsc
					ORDER BY tse.tsedsc DESC ";
			
			return $this->carregar($sql);
			
		}
	}
	
	/**
     * Pega os tipos de serviÃ§o e os fornecedores para o relatÃ³rio
     * @name pegar_tipos_fornec_prestacao_contas
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
	public function pegar_tipos_fornec_prestacao_contas($get){
		if(!empty($get['dtinicial']) && !empty($get['dtfinal'])){
			$get['dtinicial'] = pg_escape_string(formata_data_sql($get['dtinicial']));
			$get['dtfinal'] = pg_escape_string(formata_data_sql($get['dtfinal']));
			$get['agencias'] = pg_escape_string($get['agencias']);

			$sql = "SELECT
					tse.tsedsc AS tsedsc,
					fornec.fornome AS fornome
					FROM publicidade.tiposervico tse
					JOIN publicidade.itenspad item ON tse.tseid=item.tseid
					JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
					JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid
					JOIN publicidade.pad pad ON item.padid=pad.padid
					JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
					JOIN publicidade.fornecedor agencia ON cont.forid=agencia.forid
					WHERE(pgto.pipdatapagamento >= '{$get['dtinicial']}' AND pgto.pipdatapagamento <= '{$get['dtfinal']}' AND pgto.pipstatus = 'A')
					AND tse.tsestatus='A'
					AND item.ipastatus='P'
					AND pad.padstatus='A'
					AND fornec.forstatus='A'
					AND cont.cttstatus='A'
					AND agencia.forstatus='A'
					AND agencia.foragencia='1'
				    AND agencia.forid IN ({$get['agencias']})
					
					GROUP BY tse.tsedsc,fornec.fornome
					ORDER BY tse.tsedsc DESC ";
			
			return $this->carregar($sql);
		
		}
	}
	
	//converte acentuadas maiúsculas em acentuadas minúsculas para ser usada no metodo verifica_unico();
	function strtolower_utf8($string){
	  $convert_to = array(
	    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
	    "v", "w", "x", "y", "z", "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï",
	    "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "ø", "ù", "ú", "û", "ü"
	  );
	  $convert_from = array(
	    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	    "V", "W", "X", "Y", "Z", "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï",
	    "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "Ø", "Ù", "Ú", "Û", "Ü"
	  );
	
	  return str_replace($convert_from, $convert_to, $string);
	} 
}