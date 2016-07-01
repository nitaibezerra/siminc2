<?php

/**
 * Modelo de PAD
 * @author Alysson Rafael
 */
class Pad extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.pad';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('padid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'cttid' => null,
		'camid' => null,
		'padnumero' => null,
		'pademissao' => null,
		'padano' => null,
		'padnumcontrato' => null,
		'padvalor' => null,
		'padstatus' => null
	);
	
	/**
     * Gerar a combo de status
     * @name monta_combo_status
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	public function monta_combo_status(){
		
		$situacoes = array(array('codigo'=>'A','descricao'=>'Ativa'),array('codigo'=>'I','descricao'=>'Cancelada'));

		$this->monta_combo('padstatus',$situacoes,'S','Selecione...','','','','','N','padstatus','',$padstatus);
		
	}
	
	/**
     * Gerar a combo de ano
     * @name monta_combo_ano
     * @author Alysson Rafael
     * @access public
     * @param string $origem - De onde é a chamada da função
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @return void
     */
	public function monta_combo_ano($origem='',$obrigatorio='N'){
		
		$anos = array();
		if($origem == 'cadPad'){
			for($i=date('Y')-1; $i<=date('Y'); $i++){
				$anos[] = array('codigo'=>$i,'descricao'=>$i);
			}
		}
		else{
			for($i=2000; $i<=date('Y'); $i++){
				$anos[] = array('codigo'=>$i,'descricao'=>$i);
			}
		}
		
		
		$this->monta_combo('padano',$anos,'S','Selecione...','','','','',$obrigatorio,'padano','',$padano);
	}
	
	/**
     * Verifica se o registro existe
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - Dados do formulário
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['padnumero'])){
			$post['padnumero'] = str_replace('/','', $post['padnumero']);
			$post['padnumero'] = pg_escape_string($post['padnumero']);
			
			$sql = "SELECT pad.padid FROM $this->stNomeTabela pad ";
			$sql .= "WHERE pad.padnumero='{$post['padnumero']}' AND pad.padstatus='A' ";
			
			//caso seja atualização, exclui o registro atual da verificação
			if(!empty($post['padid'])){
				$post['padid'] = pg_escape_string($post['padid']);
				$sql .= "AND pad.padid <> '{$post['padid']}' ";
			}

			$padid = $this->pegaUm($sql);
			
			if($padid >= 1){
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
		
		$padnumero = str_replace('/','',pg_escape_string($padnumero));
		$pademissao = pg_escape_string(formata_data_sql($pademissao));
		$padano = pg_escape_string($padano);
		$forid = pg_escape_string($forid);
		
		if(!empty($cttnumcontrato)){
			$cttnumcontrato = str_replace('/','',pg_escape_string($cttnumcontrato));
		}
				
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
		}
				
		$camid = pg_escape_string($camid);
		$padvalor = str_replace(array('.',','),array('','.'),pg_escape_string($padvalor));
		
		if( !empty($padnumsidoc) )
		{
			$padnumsidoc = str_replace(array('/','.','-'), '', $padnumsidoc);
			$padnumsidoc = pg_escape_string($padnumsidoc);
		}
		
		if( !empty($padptres) )
		{
			$padptres = preg_replace('/[\/.-]/','',$padptres);
			$padptres = pg_escape_string($padptres);
		}
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($padid)){
			$sql = "INSERT INTO $this->stNomeTabela(camid,padnumero,pademissao,";
			$sql .= "padano,padvalor";
			if(!empty($cttnumcontrato)){
				$sql .= ",padnumcontrato";
			}
			if(!empty($cttid)){
				$sql .= ",cttid";
			}
			if( !empty($padnumsidoc) )
			{
				$sql .= ",padnumsidoc";
			}
			if( !empty($padptres) )
			{
				$sql .= ",padptres";
			}
			$sql .= ") ";
			$sql .= "VALUES('{$camid}','{$padnumero}','{$pademissao}',";
			$sql .= "'{$padano}','{$padvalor}'";
			if(!empty($cttnumcontrato)){
				$sql .= ",'{$cttnumcontrato}'";
			}
			if(!empty($cttid)){
				$sql .= ",'{$cttid}'";
			}
			if( !empty($padnumsidoc) )
			{
				$sql .= ",'{$padnumsidoc}'";
			}
			if( !empty($padptres) )
			{
				$sql .= ",'{$padptres}'";
			}
			$sql .= ")";
			
		}
		else if(!empty($padid)){
			$padid = pg_escape_string($padid);
			$sql = "UPDATE $this->stNomeTabela SET camid='{$camid}',";
			$sql .= "padnumero='{$padnumero}',pademissao='{$pademissao}',";
			$sql .= "padano='{$padano}',padvalor='{$padvalor}'";
			if(!empty($cttnumcontrato)){
				$sql .= ",padnumcontrato='{$cttnumcontrato}'";
			}
			if(!empty($cttid)){
				$sql .= ",cttid='{$cttid}'";
			}
			if( !empty($padnumsidoc) )
			{
				$sql .= ", padnumsidoc = '{$padnumsidoc}'";
			}
			if( !empty($padptres) )
			{
				$sql .= ", padptres = '{$padptres}'";
			}
			$sql .= " ";
			$sql .= "WHERE padid='{$padid}' ";
		}
		
		$sql .= "RETURNING padid ";
		
       	$padid = $this->pegaUm($sql);

       	if(!empty($padid)){
       		$this->commit();
       		return array(0=>true,1=>$padid,2=>'cadItemPad');
       	}
       	else{
       		return array(0=>false,1=>$padid,2=>'cadPad');
       	}
		
	}
	
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($padid){
		if(!empty($padid)){
			$padid = pg_escape_string($padid);
			$sql = "SELECT pad.padid,
						   contrato.cttid,
						   contrato.forid,
						   SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
						   CASE
						        WHEN CHAR_LENGTH(CAST(pad.padnumsidoc AS text)) >= 1 THEN 	
						            SUBSTR(CAST(pad.padnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(pad.padnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(pad.padnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(pad.padnumsidoc AS text) , 16 , 2)
						    END AS padnumsidoc,
						    CASE
						        WHEN CHAR_LENGTH(CAST(pad.padptres AS text)) >= 1 THEN 	
						            SUBSTR(CAST(pad.padptres AS text) , 1 , 2)||'.'||SUBSTR(CAST(pad.padptres AS text) , 3 , 3)||'.'||SUBSTR(CAST(pad.padptres AS text) , 6 , 4)
						            ||'.'||SUBSTR(CAST(pad.padptres AS text) , 10 , 4)||'.'||SUBSTR(CAST(pad.padptres AS text) , 14 , 4)
						    END   as padptres
						    , 
						   campanha.camid,
						   SUBSTR(CAST(pad.padnumero AS text),1,4)||'/'||SUBSTR(CAST(pad.padnumero AS text),5,3) AS padnumero,
						   TO_CHAR(pad.pademissao,'DD/MM/YYYY') AS pademissao,
						   pad.padano,
						   pad.padvalor,
						   pad.padstatus
						   
						   FROM $this->stNomeTabela pad 
						   LEFT JOIN publicidade.contrato contrato ON pad.cttid=contrato.cttid
						   JOIN publicidade.campanha campanha ON pad.camid=campanha.camid
						   WHERE pad.padid='{$padid}' ";
			
			return $this->pegaLinha($sql);
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

		
		$redirecionaItensPad = false;
		
		$where = " WHERE pad.padano >= '2015'  ";
		if(!empty($post['padnumero'])){
			$post['padnumero'] = str_replace('/','',pg_escape_string($post['padnumero']));
			$where .= "AND pad.padnumero = '{$post['padnumero']}' ";
		}
		if(!empty($post['forid'])){
			$post['forid'] = pg_escape_string($post['forid']);
			$where .= "AND contrato.forid='{$post['forid']}' ";
		}
		if(!empty($post['padnumcontrato'])){
			$post['padnumcontrato'] = str_replace('/','',pg_escape_string($post['padnumcontrato']));
			$where .= "AND pad.padnumcontrato = '{$post['padnumcontrato']}' ";
		}

		if(!empty($post['pipnumordembancaria']))
		{	            
			$redirecionaItensPad = true;
			
			$where .= "AND pad.padnumero in (
				SELECT 
					distinct(pad.padnumero)
				FROM 
					publicidade.pagamentoitempad pag
				
				JOIN publicidade.itenspad item ON item.ipaid=pag.ipaid
				JOIN publicidade.pad pad ON item.padid=pad.padid
				
				WHERE
					pag.pipnumordembancaria  = '{$post['pipnumordembancaria']}'
				) ";
		}
		
	if(!empty($post['fipnumfaturaagencia']))
		{	
			$redirecionaItensPad = true;
			$where .= "AND pad.padnumero in (
					
				SELECT 
					distinct(pad.padnumero)
				FROM 
					publicidade.faturamentoitempad fatura
				
				JOIN publicidade.itenspad item ON item.ipaid = fatura.ipaid
				JOIN publicidade.pad pad ON item.padid=pad.padid
				
				WHERE
					fatura.fipnumfaturaagencia  = '{$post['fipnumfaturaagencia']}'
				) ";
			
		}
                
                if(!empty($post['padvalornota']))
                    
		{	
                    
                    $post['padvalornota'] = str_replace('.','',$post['padvalornota']);
                    $post['padvalornota'] = str_replace(',','.',$post['padvalornota']);
			
			$where .= "AND pad.padnumero in (
					
				SELECT 
					distinct(pad.padnumero)
				FROM 
					publicidade.faturamentoitempad fatura
				
				JOIN publicidade.itenspad item ON item.ipaid = fatura.ipaid
				JOIN publicidade.pad pad ON item.padid=pad.padid
				
				WHERE
					item.ipavaloritem    = '{$post['padvalornota']}'
				) ";
			
		}
                
                
                
		if(!empty($post['padstatus'])){
			$post['padstatus'] = pg_escape_string($post['padstatus']);
			$where .= "AND pad.padstatus = '{$post['padstatus']}' ";
		}
		if(!empty($post['padano'])){
			$post['padano'] = pg_escape_string($post['padano']);
			$where .= "AND pad.padano = '{$post['padano']}' ";
		}
		if(!empty($post['camid'])){
			$post['camid'] = pg_escape_string($post['camid']);
			$where .= "AND pad.camid = '{$post['camid']}' ";
		}
		if(!empty($post['cttid'])){
			$post['cttid'] = pg_escape_string($post['cttid']);
			$where .= "AND pad.cttid = '{$post['cttid']}' ";
		}
		if(!empty($post['dtinicial'])){
			$post['dtinicial'] = formata_data_sql(pg_escape_string($post['dtinicial']));
			$where .= "AND pad.pademissao >= '{$post['dtinicial']}' ";
		}
		if(!empty($post['dtfinal'])){
			$post['dtfinal'] = formata_data_sql(pg_escape_string($post['dtfinal']));
			$where .= "AND pad.pademissao <= '{$post['dtfinal']}' ";
		}
                
                if(!empty($post['padvalor'])){
			$post['padvalor'] = str_replace('.','',$post['padvalor']);
			$post['padvalor'] = str_replace(',','.',$post['padvalor']);
			$where .= " AND pad.padvalor = {$post['padvalor']} ";
		}
		
		$anoAtual = date('Y');
		$anoAnterior = $anoAtual - 1;
		
		$perfil = pegaPerfilGeral();
		
		// CAso seja um dos perfis abaixo poderar aceitar a emenda
		if( in_array	(PUBLICIDADE_PERFIL_GESTOR, $perfil))
		{
			$isGestor = true;		
		}
		else
		{
			$isGestor = false;	
		}
		
		if($isGestor)
		{
			$onClick = "
				'detalhar(\''||pad.padid||'\');'    
			";	
		}
		else
		{
			$onClick = "
				CASE WHEN
		                  	(SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.padid=pad.padid) > 0 THEN
		                  	CASE WHEN
		                  		(SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.padid=pad.padid AND item.ipastatus='P') <> (SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.padid=pad.padid) AND (SUBSTR(CAST(pad.pademissao AS text),1,4)='{$anoAnterior}' OR SUBSTR(CAST(pad.pademissao AS text),1,4)='{$anoAtual}') THEN
		                  		'detalhar(\''||pad.padid||'\');' 
		                  	ELSE
		                  		'alert(\'Registro n&atilde;o pode ser editado!\')'
		                  	END
		                  ELSE   
		                  	CASE WHEN (SUBSTR(CAST(pad.pademissao AS text),1,4)='{$anoAnterior}' OR SUBSTR(CAST(pad.pademissao AS text),1,4)='{$anoAtual}') THEN
		                  		'detalhar(\''||pad.padid||'\');' 
		                  	ELSE
		                  		'alert(\'Registro n&atilde;o pode ser editado!\')'
		                  	END
		                  END 
			";
		}
	
		
		$sql = "SELECT 
				  
		 		  CASE WHEN pad.padstatus = 'A' THEN	
		
					  '<center>
	                  <a style=\"cursor:pointer;\" onclick=\"'|| 
	                  {$onClick}
	                  ||'\">
	                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar PAD\">
	                  </a>
	                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.padid=pad.padid AND item.ipastatus='P') = 0 AND (SUBSTR(CAST(pad.pademissao AS text),1,4)='{$anoAtual}') THEN ' if(confirm(\'Deseja cancelar a PAD?\')){remover(\''||pad.padid||'\');}' ELSE 'alert(\'Registro n&atilde;o pode ser cancelado!\');' END || '\">
	                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Cancelar PAD\">
	                  </a>
	                  <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||pad.padid||'\');\">
	                  <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar dados da PAD\">
	                  </a>
	                  </center>'

                  ELSE
                  
	                  '<center>
	                  <a style=\"cursor:pointer;\" onclick=\"visualizar(\''||pad.padid||'\');\">
	                  <img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar dados da PAD\">
	                  </a>
	                  </center>'
                  
                  END as acao,
                  
                  CASE WHEN pad.padstatus = 'A' THEN
                  	'Ativa'
                  ELSE
                  	'Cancelada'
                  END AS padstatus,
                  
                  SUBSTR(CAST(pad.padnumero AS text),1,4)||'/'||SUBSTR(CAST(pad.padnumero AS text),5,3) AS padnumero,
                  fornecedor.fornome AS fornome,
                  SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
                  campanha.camtitulo AS camtitulo,
                  pad.padano||' ' AS padano,
                  pad.padvalor AS padvalor
                  
                  FROM $this->stNomeTabela pad 
                  LEFT JOIN publicidade.contrato contrato ON pad.cttid=contrato.cttid
                  LEFT JOIN publicidade.fornecedor fornecedor ON contrato.forid=fornecedor.forid
                  JOIN publicidade.campanha campanha ON pad.camid=campanha.camid
                  {$where} 
                  ORDER BY pad.padnumero ";
                  
                  //echo $sql;
		if( $redirecionaItensPad && $isGestor )
		{
			$dados = $this->carregar($sql);
			
			if( is_array($dados) )
			{
				
				$sqlItem = "SELECT 
				
		 		  pad.padid
                  
                  FROM $this->stNomeTabela pad 
                  LEFT JOIN publicidade.contrato contrato ON pad.cttid=contrato.cttid
                  LEFT JOIN publicidade.fornecedor fornecedor ON contrato.forid=fornecedor.forid
                  JOIN publicidade.campanha campanha ON pad.camid=campanha.camid
                  {$where} 
                  ORDER BY pad.padnumero ";
                  //ver($sqlItem,d);
				
				
				$ob		= ($post['pipnumordembancaria']) ? $post['pipnumordembancaria'] : "''" ;
				$nota	= ($post['fipnumfaturaagencia']) ? $post['fipnumfaturaagencia'] : "''" ;
				
				$id = $this->carregar($sqlItem);
				if( is_array($id) )
				{
					$id = $id[0][padid];	
				}
				
				$dados = $dados[0];
				print_r( $id );
				echo "<script>filtraParaItens( $id, $nota, $ob )</script>";
				die('redirecionae!');
			}
			else 
			{
				$cabecalho = array('A&ccedil;&atilde;o','Status','N&uacute;mero da PAD','Ag&ecirc;ncia','Contrato','Campanha','Ano','Valor Total');
				$this->monta_lista($sql,$cabecalho,10,50,false,"center");
			}
			
		}
		else
		{
			$cabecalho = array('A&ccedil;&atilde;o','Status','N&uacute;mero da PAD','Ag&ecirc;ncia','Contrato','Campanha','Ano','Valor Total');
			$this->monta_lista($sql,$cabecalho,10,50,false,"center");	
		}

		
		
	}
	
	/**
     * Inativa registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro pai
     * @return void
     */
	public function inativar($padid){
		if(!empty($padid)){
			$padid = pg_escape_string($padid);
			$sql = "UPDATE $this->stNomeTabela SET padstatus='I' WHERE padid='{$padid}' ";
			$this->executar($sql);
			$this->commit();
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
		if(!empty($post['dtinicial']) || !empty($post['dtfinal'])){
			
			$auxInicial = explode('/',$post['dtinicial']);
			$auxFinal = explode('/',$post['dtfinal']);
			
			if(!empty($post['dtinicial'])){
				$post['dtinicial'] = strtotime(formata_data_sql($post['dtinicial']));
			}
			if(!empty($post['dtfinal'])){
				$post['dtfinal'] = strtotime(formata_data_sql($post['dtfinal']));
			}
			
			$erro = '';
			
			if(!empty($auxInicial[2]) && $auxInicial[2] < 2000){
				$erro = 'anoinicialinvalido';
			}
			else if(!empty($auxFinal[2]) && $auxFinal[2] < 2000){
				$erro = 'anofinalinvalido';
			}
			
			if(!empty($post['dtfinal']) && !empty($post['dtinicial'])){
				if($post['dtfinal'] < $post['dtinicial']){
					$erro = 'datamenor';
				}
			}
			
			return $erro;
		}
	}
	
	/**
     * Pega dados para o relatório memorando
     * @name pegar_dados_memorando
     * @author Alysson Rafael
     * @access public
     * @param string $padnumero - Número da PAD
     * @return array
     */
	public function pegar_dados_memorando($padnumero){
		if(!empty($padnumero)){
			$padnumero = str_replace('/','',pg_escape_string($padnumero));
			$sql = "SELECT 
					fornec.fornome AS fornome,
					SUBSTR(CAST(contrato.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(contrato.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
						   
				    CASE
				        WHEN CHAR_LENGTH(CAST(pad.padnumsidoc AS text)) >= 1 THEN 	
				            SUBSTR(CAST(pad.padnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(pad.padnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(pad.padnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(pad.padnumsidoc AS text) , 16 , 2)
				    END AS padnumsidoc,
				    pad.padptres,
					pad.padano AS padano
					FROM publicidade.fornecedor fornec
					LEFT JOIN publicidade.contrato contrato ON fornec.forid=contrato.forid
					LEFT JOIN $this->stNomeTabela pad ON contrato.cttid=pad.cttid
					WHERE pad.padnumero='{$padnumero}' ";
			
			$resultado = $this->carregar($sql);
			return $resultado[0];
		}
	}
	
	/**
     * Pega dados para o relatório execução fiscal
     * @name pegar_dados_execucao_fiscal
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */	
	public function pegar_dados_execucao_fiscal($get){
		if(!empty($get['datainicial']) && !empty($get['datafinal']) && !empty($get['agencias'])){
			$get['datainicial'] = formata_data_sql(pg_escape_string($get['datainicial']));
			$get['datafinal'] = formata_data_sql(pg_escape_string($get['datafinal']));
			
			//explode a string com os ids das ag�ncias
			$aux = explode(',',$get['agencias']);
			$retorno = array();
			if(is_array($aux) && count($aux) >= 1){
				
				foreach($aux as $key => $value){
					$value = pg_escape_string($value);
					$sql = "SELECT
							SUBSTR(CAST(MIN(pad.padnumero) AS text),1,4)||'/'||SUBSTR(CAST(MIN(pad.padnumero) AS text),5,3) AS padnumeroinicial,
							SUBSTR(CAST(MAX(pad.padnumero) AS text),1,4)||'/'||SUBSTR(CAST(MAX(pad.padnumero) AS text),5,3) AS padnumerofinal,
							fornec.fornome AS fornome 
							FROM $this->stNomeTabela pad 
							LEFT JOIN publicidade.contrato cont ON pad.cttid=cont.cttid 
							LEFT JOIN publicidade.fornecedor fornec ON cont.forid=fornec.forid
							LEFT JOIN publicidade.itenspad item ON pad.padid=item.padid
							LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
							LEFT JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
							WHERE (
							(fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A') OR
							(pgto.pipdatapagamento >= '{$get['datainicial']}' AND pgto.pipdatapagamento <= '{$get['datafinal']}' AND pgto.pipstatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'F') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'P')
							)
							AND cont.forid = '{$value}' 
							AND pad.padstatus = 'A' 
							AND cont.cttstatus = 'A'
							AND fornec.forstatus='A'
							GROUP BY fornec.fornome ";
					
					$resultado = $this->carregar($sql);
					
					$retorno[$value] = $resultado[0];
					
				}
			}
			
			return $retorno;
			
		}
	}
	
	/**
     * Pega qtd de pads e os ids das mesmas
     * @name pegar_qtd_execucao_fiscal
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */	
	public function pegar_qtd_execucao_fiscal($get){
		if(!empty($get['datainicial']) && !empty($get['datafinal']) && !empty($get['agencias'])){
			$get['datainicial'] = formata_data_sql(pg_escape_string($get['datainicial']));
			$get['datafinal'] = formata_data_sql(pg_escape_string($get['datafinal']));
			
			//explode a string com os ids das agências
			$aux = explode(',',$get['agencias']);
			$retorno = array();
			if(is_array($aux) && count($aux) >= 1){
				
				foreach($aux as $key => $value){
					$value = pg_escape_string($value);
					$sql = "SELECT DISTINCT
							pad.padid
							FROM $this->stNomeTabela pad
							LEFT JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
							LEFT JOIN publicidade.itenspad item ON pad.padid=item.padid
							LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
							LEFT JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
							WHERE(
							(fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A') OR
							(pgto.pipdatapagamento >= '{$get['datainicial']}' AND pgto.pipdatapagamento <= '{$get['datafinal']}' AND pgto.pipstatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'F') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'P')
							)
							AND cont.forid='{$value}' 
							AND pad.padstatus = 'A' 
							AND cont.cttstatus = 'A' ";
					
					$resultado = $this->carregar($sql);
					if($resultado){
						$retorno[$value] = $resultado;
					}
				}
			}
			
			return $retorno;
		}
	}
	
	
	
	/**
     * Pega dados para o relatório espelho da pad
     * @name pegar_dados_espelho_pad
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */	
	public function pegar_dados_espelho_pad($get){
		if(!empty($get['padid'])){
			$get['padid'] = pg_escape_string($get['padid']);
			
			$sql = "SELECT SUBSTR(CAST(pad.padnumero AS text),1,4)||'/'||SUBSTR(CAST(pad.padnumero AS text),5,3) AS padnumero,
						   TO_CHAR(pad.pademissao,'DD/MM/YYYY') AS pademissao,
						   CASE
						        WHEN CHAR_LENGTH(CAST(pad.padnumsidoc AS text)) >= 1 THEN 	
						            SUBSTR(CAST(pad.padnumsidoc AS text) , 1 , 5)||'.'||SUBSTR(CAST(pad.padnumsidoc AS text) , 6 , 6)||'/'||SUBSTR(CAST(pad.padnumsidoc AS text) , 12 , 4)||'-'||SUBSTR(CAST(pad.padnumsidoc AS text) , 16 , 2)
						   ELSE
						   		''
						   END AS padnumsidoc,
						   CASE
						        WHEN CHAR_LENGTH(CAST(pad.padptres AS text)) >= 1 THEN 	
						            SUBSTR(CAST(pad.padptres AS text) , 1 , 2)||'.'||SUBSTR(CAST(pad.padptres AS text) , 3 , 3)||'.'||SUBSTR(CAST(pad.padptres AS text) , 6 , 4)
						            ||'.'||SUBSTR(CAST(pad.padptres AS text) , 10 , 4)||'.'||SUBSTR(CAST(pad.padptres AS text) , 14 , 4)
						   ELSE
						   		''   
						   END   as padptres,
						   
						   SUBSTR(CAST(cont.cttnumcontrato AS text) , 1 , 3)||'/'||SUBSTR(CAST(cont.cttnumcontrato AS text) , 4 , 4) AS cttnumcontrato,
						   fornec.fornome AS fornome,
						   camp.camtitulo AS camtitulo,
						   org.orgdsc AS orgdsc
						   FROM publicidade.fornecedor fornec
						   JOIN publicidade.contrato cont ON fornec.forid=cont.forid
						   JOIN $this->stNomeTabela pad ON cont.cttid=pad.cttid
						   JOIN publicidade.campanha camp ON pad.camid=camp.camid
						   JOIN publicidade.vworgao org ON camp.orgid=org.orgid
						   LEFT JOIN publicidade.itenspad item ON pad.padid=item.padid
						   WHERE pad.padid='{$get['padid']}'
						   GROUP BY padnumero,pademissao,padnumsidoc,pad.padptres,cttnumcontrato,fornome,camtitulo,orgdsc ";
			
			return $this->pegaLinha($sql);
			
			
		}
	}

	
}