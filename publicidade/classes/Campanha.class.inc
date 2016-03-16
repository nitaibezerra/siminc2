<?php

/**
 * Modelo de Campanha
 * @author Alysson Rafael
 */
class Campanha extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.campanha';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('camid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'camdsc' => null,
		'camtitulo' => null,
		'camstatus' => null,
		'orgid' => null
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
		
		$where = " WHERE campanha.camstatus='A' ";
		if(!empty($post['camtitulo'])){
			$post['camtitulo'] = tirar_acentos(pg_escape_string($post['camtitulo']));
			$where .= " AND(TRANSLATE(campanha.camtitulo, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['camtitulo']}%' OR campanha.camtitulo ILIKE '%{$post['camtitulo']}%') ";
			//$where .= " AND campanha.camtitulo ILIKE '%{$post['camtitulo']}%' OR campanha.camtitulo ILIKE '%{$post['camtitulo']}%') ";
		}
		if(!empty($post['orgid'])){
			$post['orgid'] = pg_escape_string($post['orgid']);
			$where .= " AND campanha.orgid = '{$post['orgid']}' ";
		}
		
		
		$sql = "SELECT 
				  '<center>
                  <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||campanha.camid||'\');\">
                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                  </a>
                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM publicidade.pad pad WHERE pad.camid=campanha.camid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||campanha.camid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do!\');' END || '\">
                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                  </a>
                  </center>' as acao,
                  campanha.camtitulo,
                  orgao.orgdsc
                  
                  FROM $this->stNomeTabela campanha 
                  JOIN publicidade.vworgao orgao ON campanha.orgid=orgao.orgid
                  {$where} 
                  ORDER BY campanha.camtitulo ";
		
		$cabecalho = array('A��o','Campanha','�rg�o Solicitante');
		$this->monta_lista($sql,$cabecalho,10,50,false,"center",'S','','',array('center','center','center'));
		
	}
	
	/**
     * Verifica registro único
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - Dados do formulário
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['camtitulo'])){
			$post['camtitulo'] = pg_escape_string(strtolower($post['camtitulo']));
			
			$sql = "SELECT campanha.camid FROM $this->stNomeTabela campanha ";
			$sql .= "WHERE LOWER(campanha.camtitulo)='{$post['camtitulo']}' AND campanha.camstatus='A' ";
			
			//caso seja atualização, exclui o registro atual da verificação
			if(!empty($post['camid'])){
				$post['camid'] = pg_escape_string($post['camid']);
				$sql .= "AND campanha.camid <> '{$post['camid']}' ";
			}
			
			$camid = $this->pegaUm($sql);
			
			if($camid >= 1){
				return true;
			}
			else{
				return false;
			}
		}
	}
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param array $camid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($camid){
		if(!empty($camid)){
			$camid = pg_escape_string($camid);
			$sql = "SELECT campanha.camid,
						   campanha.camtitulo,
						   campanha.camstatus,
						   campanha.orgid,	
						   campanha.camdsc
						   FROM $this->stNomeTabela campanha 
						   WHERE campanha.camid='{$camid}' ";
			return $this->pegaLinha($sql);
		}
	}
	
	/**
     * Gerar a combo de campanha
     * @name monta_combo_campanha
     * @author Alysson Rafael
     * @access public
     * @param string $camid       - Qual option receberá o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @param string $origem      - De onde foi chamada a função
     * @return void
     */
	public function monta_combo_campanha($camid='',$obrigatorio='N',$origem=''){
		
		$sql = "SELECT camid AS codigo,camtitulo AS descricao FROM $this->stNomeTabela WHERE camstatus='A' ORDER BY camtitulo ";

		$resultado = $this->carregar($sql);
		
		if(!is_array($resultado) || count($resultado) < 1){
			$resultado = array();
		}

		$this->monta_combo('camid',$resultado,'S','Selecione...','','','','',$obrigatorio,'camid','',$camid);
		
	}
	
	
	/**
     * Inativa registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $orgid - Identificador do registro 
     * @return void
     */
	public function inativar($camid){
		if(!empty($camid)){
			$camid = pg_escape_string($camid);
			$sql = "UPDATE $this->stNomeTabela SET camstatus='I' WHERE camid='{$camid}' ";
			$this->executar($sql);
			$this->commit();
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
		
		$camtitulo = pg_escape_string($camtitulo);
		$orgid = pg_escape_string($orgid);
		$camdsc = pg_escape_string($camdsc);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($camid)){
			$sql = "INSERT INTO $this->stNomeTabela(camtitulo,orgid,camdsc) ";
			$sql .= "VALUES('{$camtitulo}','{$orgid}','{$camdsc}') ";
			
		}
		else if(!empty($camid)){
			$camid = pg_escape_string($camid);
			$sql = "UPDATE $this->stNomeTabela SET camtitulo='{$camtitulo}',orgid='{$orgid}',camdsc='{$camdsc}' ";
			$sql .= "WHERE camid='{$camid}' ";
		}
		
		$sql .= "RETURNING camid ";
		
       	$camid = $this->pegaUm($sql);

       	if(!empty($camid)){
       		$this->commit();
       		return array(0=>true,1=>$camid,2=>'listaCampanha');
       	}
       	else{
       		return array(0=>false,1=>$camid,2=>'listaCampanha');
       	}
		
	}
	
	/**
     * Pega as campanhas com o total de cada uma 
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

			$aux = explode(',',$get['agencias']);
			
			$retorno = array();
			if(is_array($aux) && count($aux) >= 1){
				foreach($aux as $key => $value){
					$value = pg_escape_string($value);
					
					$sql = "SELECT camp.camid AS camid,camp.camtitulo AS camtitulo,SUM(case WHEN trim(pgto.pipvalorfaturado)::numeric > 0 then trim(pgto.pipvalorfaturado)::numeric when coalesce(fatura.fipvalortotal,0) > 0 then fatura.fipvalortotal  else (item.ipavalorservico + item.ipavalorhonorario) - coalesce(gipvalor,0) end) AS valor
							FROM publicidade.campanha camp
							JOIN publicidade.pad pad ON camp.camid=pad.camid
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
							JOIN publicidade.itenspad item ON pad.padid=item.padid
							LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
							LEFT JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
                                                        left join publicidade.glosaitempad glo on glo.ipaid = item.ipaid
							WHERE (
							(fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A') OR
							(pgto.pipdatapagamento >= '{$get['datainicial']}' AND pgto.pipdatapagamento <= '{$get['datafinal']}' AND pgto.pipstatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'A') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'F') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'P') OR
							(item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'G')
							)
							AND cont.forid='{$value}' 
							AND cont.cttstatus='A'
							AND pad.padstatus='A'
							AND camp.camstatus='A'
							GROUP BY camp.camid,camp.camtitulo ";
					
					$resultado = $this->carregar($sql);
					if($resultado){

						foreach($resultado as $key2 => $value2){
							if(!array_key_exists($value2['camid'], $retorno)){
								$retorno[$value2['camid']] = array('camtitulo'=>$value2['camtitulo'],'valor'=>$value2['valor']);
							}
							else{
								$soma = new Math($retorno[$value2['camid']]['valor'], $value2['valor'],2);
								$retorno[$value2['camid']]['valor'] = $soma->sum()->getResult();
							}
						}
                       
						//verifica se existe algum item com glosa para abater no total da campanha correspondente
//						if(is_array($retorno) && count($retorno)){
//							foreach($retorno as $key2 => $value2){
//								$sql = "SELECT item.ipavalortotal,fatura.fipvalortotal 
//								        FROM publicidade.pad pad
//								        JOIN publicidade.itenspad item ON pad.padid=item.padid
//								        JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
//								        WHERE pad.camid='{$key2}'
//								        AND( 
//								        (item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus = 'G') OR
//								        (fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND item.ipastatus = 'G') 
//								        )";
//								$res = $this->carregar($sql);
//								if(is_array($res) && count($res)){
//									foreach($res as $key3 => $value3){
//										$subtrai = new Math($retorno[$key2]['valor'], $value3['ipavalortotal'],2);
//										$retorno[$key2]['valor'] = $subtrai->sub()->getResult();
//										$soma = new Math($retorno[$key2]['valor'], $value3['fipvalortotal'],2);
//										$retorno[$key2]['valor'] = $soma->sum()->getResult();
//									}
//								}
//							}
//						}
						
					}
				}
			}
			//ver($retorno,d);
			return $retorno;
			
		}
	}
	

	
}