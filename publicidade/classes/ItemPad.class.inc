<?php

/**
 * Modelo de Item da PAD
 * @author Alysson Rafael
 */
class ItemPad extends Modelo {

    /**
     * Constante para status APROVADO
     */
    const ST_APROVADO = 'A';

    /**
     * Constante para status FATURADO
     */
    const ST_FATURADO = 'F';

    /**
     * Constante para status GLOSADO
     */
    const ST_GLOSADO = 'G';

    /**
     * Constante para status PAGO
     */
    const ST_PAGO = 'P';

    /**
     * Constante para status CANCELADO
     */
    const ST_CANCELADO = 'C';

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'publicidade.itenspad';

    /**
     * Chave primária
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('ipaid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ipaid' => null,
        'tseid' => null,
        'catid' => null,
        'padid' => null,
        'tppid' => null,
        'ipanumitempad' => null,
        'ipadsc' => null,
        'ipaqtd' => null,
        'ipavalorservico' => null,
        'ipahonorario' => null,
        'ipavalorhonorario' => null,
        'ipastatus' => null,
        'forid' => null,
        'ipavalortotal' => null,
        'ipadesconto' => null,
        'ipavaloritem' => null,
    );

    /**
     * Lista de status possíveis para o item da PAD
     * 
     * @name $listaStatusPossivel
     * @var array
     * @access protected
     */
    protected $listaStatusPossivel = array(
        self::ST_APROVADO => 'Aprovado',
        self::ST_FATURADO => 'Faturado',
        self::ST_GLOSADO => 'Glosado',
        self::ST_PAGO => 'Pago',
        self::ST_CANCELADO => 'Cancelado',
    );

    /**
     * Pega o último número de um item
     * @name pega_ultimo_numero
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro pai
     * @return string
     */
    public function pega_ultimo_numero($padid) {
        if (!empty($padid)) {
            $padid = pg_escape_string($padid);
            $sql = "SELECT ipanumitempad FROM $this->stNomeTabela ";
            $sql .= "WHERE padid='{$padid}' ORDER BY ipaid DESC ";
            $resultado = $this->pegaUm($sql);
            return $resultado;
        }
    }

    /**
     *  Pega data de emissao do pad - pai do item
     * @name pega_data_pad
     * @author Robson Moura
     * @since 10/04/2013
     * @access public  
     * @param int $padid - Identificador do registro pai
     * @return string
     */
    public function pega_data_pad($padid) {
        if (!empty($padid)) {
            $padid = pg_escape_string($padid);
            $sql = "SELECT pademissao FROM publicidade.pad ";
            $sql .= "WHERE padid='{$padid}' ";
            $resultado = $this->pegaUm($sql);
            return $resultado;
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
    public function salvar($post) {
        extract($post);

        $ipanumitempad = pg_escape_string($post['ipanumitempad']);
        $ipadsc = pg_escape_string($post['ipadsc']);
        $forid = $post['forid'] ? pg_escape_string($post['forid']) : pg_escape_string($post['forid_fornecedor']);     
        $tseid = pg_escape_string($post['tseid']);
        $catid = pg_escape_string($post['catid']);
        $tppid = pg_escape_string($post['tppid']);
        $ipaqtd = str_replace('.', '', pg_escape_string($post['ipaqtd']));
        $ipavalorservico = str_replace(array('.', ','), array('', '.'), pg_escape_string($post['ipavalorservico']));
        $ipadesconto = str_replace(array('.', ','), array('', '.'), pg_escape_string($post['ipadesconto']));
        $ipahonorario = pg_escape_string($post['ipahonorario']);
        $ipavalorhonorario = str_replace(array('.', ','), array('', '.'), pg_escape_string($post['ipavalorhonorario']));
        $ipavaloritem = str_replace(array('.', ','), array('', '.'), pg_escape_string($post['ipavaloritem']));
        $ipavalortotal = str_replace(array('.', ','), array('', '.'), pg_escape_string($post['ipavalortotal']));
        $padid = pg_escape_string($id);

        //Pega data da emiss�o do pad pai
        $ipadata = $this->pega_data_pad($padid);

        //caso o id esteja vazio, a operação será de insert
        //caso não esteja vazio, a operação será de update
        if (empty($ipaid)) {
            $sql = "INSERT INTO $this->stNomeTabela(tseid,catid,padid,tppid,ipanumitempad,ipadsc,ipaqtd,";
            $sql .= "ipavalorservico,ipahonorario,ipavalorhonorario,forid,ipavalortotal,ipadesconto,ipavaloritem, ipadata) ";
            $sql .= "VALUES('{$tseid}','{$catid}','{$padid}','{$tppid}','{$ipanumitempad}','{$ipadsc}','{$ipaqtd}',";
            $sql .= "'{$ipavalorservico}','{$ipahonorario}','{$ipavalorhonorario}','{$forid}','{$ipavalortotal}',";
            $sql .= "'{$ipadesconto}','{$ipavaloritem}','{$ipadata}') ";
        } else if (!empty($ipaid)) {
            $ipaid = pg_escape_string($ipaid);
            $sql = "UPDATE $this->stNomeTabela SET tseid='{$tseid}',catid='{$catid}',";
            $sql .= "padid='{$padid}',tppid='{$tppid}',";
            $sql .= "ipanumitempad='{$ipanumitempad}',ipadsc='{$ipadsc}',ipaqtd='{$ipaqtd}',";
            $sql .= "ipavalorservico='{$ipavalorservico}',ipahonorario='{$ipahonorario}',";
            $sql .= "ipavalorhonorario='{$ipavalorhonorario}',forid='{$forid}',ipavalortotal='{$ipavalortotal}',";
            $sql .= "ipadesconto='{$ipadesconto}',ipavaloritem='{$ipavaloritem}' ";
            $sql .= "WHERE ipaid='{$ipaid}' ";
        }

        $sql .= "RETURNING ipaid ";

        $ipaid = $this->pegaUm($sql);

        if (!empty($ipaid)) {
            $this->commit();
            return array(0 => true, 1 => $padid, 2 => $ipaid, 3 => 'cadItemPad');
        } else {
            return array(0 => false, 1 => $padid, 2 => $ipaid, 3 => 'cadItemPad');
        }
    }

    /**
     * Pega a qtd de itens de uma pad
     * @name pega_qtd_itens
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro pai
     * @return int
     */
    public function pega_qtd_itens($padid) {
        if (!empty($padid)) {
            $padid = pg_escape_string($padid);
            $sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE padid='{$padid}' ";
            $resultado = $this->pegaUm($sql);
            return $resultado;
        }
    }

    /**
     * Verifica se tem o perfil gestor
     * @name getIsGestor
     * @author Elias.oliveira
     * @param none
     * @access public
     * @return boolean
     */
    public function getIsGestor() {
        $perfil = pegaPerfilGeral();

        // CAso seja um dos perfis abaixo poderar aceitar a emenda
        if (in_array(PUBLICIDADE_PERFIL_GESTOR, $perfil)) {
            $isGestor = true;
        } else {
            $isGestor = false;
        }
        return $isGestor;
    }

    /**
     * Listagem dos dados
     * @name listar_por_id
     * @author Alysson Rafael
     * @param int $padid - identificador do registro pai
     * @access public
     * @return void
     */
    public function listar_por_id($padid, $readOnly, $arrParams = array()) {

        // Verifica se possui o perfil gestor
        $isGestor = $this->getIsGestor();

        if ($isGestor) {
            $visualizar = "
			<a style=\"cursor:pointer;\" onclick=\"detalharItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
			
	                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar Item PAD\">
	            </a>
			
				<a style=\"cursor:pointer;\" onclick=\"visualizar(\''||item.ipanumitempad||'\');\">
	            	<img src=\"/imagens/icone_lupa.png \" border=0 title=\"Visualizar dados do Item PAD\">
	            </a>
			";
        } else {
            $visualizar = " ";
        }


        if (!empty($padid)) {
            $padid = pg_escape_string($padid);

            $where = " WHERE item.padid='{$padid}' ";

            if (count($arrParams)) {

                $nota = $arrParams['fipnumfaturaagencia'];
                $ob = $arrParams['pipnumordembancaria'];

                if ($nota) {
                    $where .= "	AND
									fipnumfaturaagencia  = '{$nota}' ";
                }
                if ($ob) {
                    $where .= "	AND
				 					pipnumordembancaria = '{$ob}'";
                }
            }

            // Valida se existem itens marcados de outras páginas
            if (count($_SESSION['ITENS_MARCADOS_LIST'])) {

                $arrItem = $_SESSION['ITENS_MARCADOS_LIST'];
                $arrItem = implode(",", $arrItem);

                $checkBox = "
			' ||
					CASE 
						WHEN 
							item.ipaid in ( {$arrItem} )
						THEN 
							'<input type=\"checkbox\" checked name=\"checkitempad\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\" id=\"checkitempad\" value=\" ' || item.ipaid || ' \">'	
						
						ELSE
							'<input type=\"checkbox\"  name=\"checkitempad\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\" id=\"checkitempad\" value=\" ' || item.ipaid || ' \">'
					END
				|| ' ' ||
				";
            } else {
                $checkBox = "  <input type=\"checkbox\"  name=\"checkitempad\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\" id=\"checkitempad\" value=\" ' || item.ipaid || ' \"> ' ||";
            }


            if ($readOnly != 'S' && $_SESSION['visualizar'] != 'S') {

                $sql = "SELECT 
					  '<center>
					  {$visualizar}
	                   $checkBox 
	                  
	                  CASE WHEN item.ipastatus = 'A' THEN";

                // CASO SEJA GESTOR JÁ VAI TER ADICIONADO O BOTÃO PARA ALTERAR, ENTÃO NÃO ADICIONA DENOVO
                if (!$isGestor) {
                    $sql .="'<a style=\"cursor:pointer;\" onclick=\"detalharItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
		                  	  <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar Item da PAD\">
		                      </a>' ||";
                }
                $sql .= "'<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                      <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
	                      </a>
	                      <a style=\"cursor:pointer;\" onclick=\"cancelarReativarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                      <img src=\"/imagens/pendente.gif \" border=0 title=\"Cancelar/Reativar Item da PAD\">
	                      </a>'
	                      ELSE 
	                      CASE WHEN item.ipastatus = 'C' THEN 
	                          '<a style=\"cursor:pointer;\" onclick=\"cancelarReativarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                          <img src=\"/imagens/pendente.gif \" border=0 title=\"Cancelar/Reativar Item da PAD\">
	                          </a>'
	                          ELSE
	                          CASE WHEN item.ipastatus = 'F' THEN
	                              '<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
	                              </a>
	                              <a style=\"cursor:pointer;\" onclick=\"glosarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/valida6.gif \" border=0 title=\"Glosar Item da PAD\">
	                              </a>
	                              <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                              </a>
                                      <a style=\"cursor:pointer;\" onclick=\"cancelarFaturamento(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/exclui_p.gif \" border=0 title=\"Cancelar Faturamento do Item da PAD\">
	                                      </a>'
	                              ELSE
	                              CASE WHEN item.ipastatus = 'G' THEN
	                                  '<a style=\"cursor:pointer;\" onclick=\"glosarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                  <img src=\"/imagens/valida6.gif \" border=0 title=\"Glosar Item da PAD\">
	                                  </a>
	                                  <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                  <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                                  </a>'
	                                  ELSE
	                                  CASE WHEN item.ipastatus = 'P' THEN
	                                      '<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
			                              <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
			                              </a>
	                                      <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                                      </a>
                                              <a style=\"cursor:pointer;\" onclick=\"cancelarPagamento(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/exclui_p.gif \" border=0 title=\"Cancelar Pagamento do Item da PAD\">
	                                      </a>'
	                                  END
	                              END
	                          END
	                      END
	                  END
	                  
					  ||'               
	                  </center>' as acao,
		              CASE WHEN item.ipastatus = 'A' THEN 'Aprovado' 
					      ELSE 
					  	  CASE WHEN item.ipastatus = 'C' THEN 'Cancelado'
					  	      ELSE
					  		  CASE WHEN item.ipastatus = 'F' THEN 'Faturado'
							      ELSE
								  CASE WHEN item.ipastatus = 'P' THEN 'Pago'
								      ELSE
									  CASE WHEN item.ipastatus = 'G' THEN 'Glosado'
									  END
							      END
					  		  END
					      END	
		              END AS situacao,
		              CASE 
						    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 10 ) THEN
							
						        substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '000' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
						    ELSE
							CASE 
							    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 100 ) THEN
						
								substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '00' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
							    ELSE
								CASE 
								    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 1000 ) THEN
						
									substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '0' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
									
								END
							END
							
						END as ipanumitempad,
		              fornec.fornome,
		              tipo.tsedsc,
		              item.ipaqtd||' ',
		              item.ipavalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.glosaitempad glo WHERE glo.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(glosa.gipvalor AS text) END AS gipvalor,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(fatura.fipdataftura,'DD/MM/YYYY') END AS fipdataftura,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE fatura.fipnumfaturaagencia||' ' END AS fipnumfaturaagencia,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(fatura.fipvalortotal AS text) END AS fipvalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(pag.pipdatapagamento,'DD/MM/YYYY') END AS pipdatapagamento,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipnumordembancaria||' ' AS text) END AS pipnumordembancaria,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipvalorfaturado AS text) END AS pipvalorfaturado
		              
	                  FROM $this->stNomeTabela item
	                  JOIN publicidade.pad pad ON item.padid=pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid and fatura.fipstatus = 'A'
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid and pag.pipstatus = 'A'
	                  {$where} 
	                  ORDER BY ipanumitempad 
	                  
	                  ";
            } else if ($readOnly == 'S' && $_SESSION['visualizar'] != 'S') {
                $sql = "SELECT 
					  '<center>
					  {$visualizar}
					  -
	                  </center>' as acao,
		              CASE WHEN item.ipastatus = 'A' THEN 'Aprovado' 
					      ELSE 
					  	  CASE WHEN item.ipastatus = 'C' THEN 'Cancelado'
					  	      ELSE
					  		  CASE WHEN item.ipastatus = 'F' THEN 'Faturado'
							      ELSE
								  CASE WHEN item.ipastatus = 'P' THEN 'Pago'
								      ELSE
									  CASE WHEN item.ipastatus = 'G' THEN 'Glosado'
									  END
							      END
					  		  END
					      END	
		              END AS situacao,
		               CASE 
						    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 10 ) THEN
							
						        substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '000' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
						    ELSE
							CASE 
							    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 100 ) THEN
						
								substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '00' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
							    ELSE
								CASE 
								    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 1000 ) THEN
						
									substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '0' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
									
								END
							END
							
						END as ipanumitempad,
		              fornec.fornome,
		              tipo.tsedsc,
		              item.ipaqtd,
		              item.ipavalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.glosaitempad glo WHERE glo.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(glosa.gipvalor AS text) END AS gipvalor,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(fatura.fipdataftura,'DD/MM/YYYY') END AS fipdataftura,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE fatura.fipnumfaturaagencia END AS fipnumfaturaagencia,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(fatura.fipvalortotal AS text) END AS fipvalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(pag.pipdatapagamento,'DD/MM/YYYY') END AS pipdatapagamento,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipnumordembancaria AS text) END AS pipnumordembancaria,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipvalorfaturado AS text) END AS pipvalorfaturado
		              
	                  FROM $this->stNomeTabela item
	                  JOIN publicidade.pad pad ON item.padid=pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid and fatura.fipstatus = 'A'
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid and pag.pipstatus = 'A'
	                  {$where} 
	                  ORDER BY ipanumitempad ";
            } else if ($_SESSION['visualizar'] == 'S') {

                $sql = "SELECT
					  '<center>
					  {$visualizar}
	                  $checkBox 
	                  
	                  CASE WHEN item.ipastatus = 'A' THEN
	                      '<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                      <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
	                      </a>'
	                      ELSE 
	                      CASE WHEN item.ipastatus = 'C' THEN 
	                          ''
	                          ELSE
	                          CASE WHEN item.ipastatus = 'F' THEN
	                              '<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
	                              </a>
	                              <a style=\"cursor:pointer;\" onclick=\"glosarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/valida6.gif \" border=0 title=\"Glosar Item da PAD\">
	                              </a>
	                              <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                              <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                              </a>
                                      <a style=\"cursor:pointer;\" onclick=\"cancelarFaturamento(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/exclui_p.gif \" border=0 title=\"Cancelar Faturamento do Item da PAD\">
	                                      </a>'
	                              ELSE
	                              CASE WHEN item.ipastatus = 'G' THEN
	                                  '<a style=\"cursor:pointer;\" onclick=\"glosarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                  <img src=\"/imagens/valida6.gif \" border=0 title=\"Glosar Item da PAD\">
	                                  </a>
	                                  <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                  <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                                  </a>'
	                                  ELSE
	                                  CASE WHEN item.ipastatus = 'P' THEN
	                                      '<!--<a style=\"cursor:pointer;\" onclick=\"faturarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
			                              <img src=\"/imagens/star.gif \" border=0 title=\"Faturar Item da PAD\">
			                              </a>-->
	                                      <a style=\"cursor:pointer;\" onclick=\"pagarItem(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/cifrao.jpg \" border=0 title=\"Pagar Item da PAD\">
	                                      </a>
                                              <a style=\"cursor:pointer;\" onclick=\"cancelarPagamento(\''||item.ipaid||'\',\''||item.ipastatus||'\');\">
	                                      <img src=\"/imagens/exclui_p.gif \" border=0 title=\"Cancelar Pagamento do Item da PAD\">
	                                      </a>'
	                                  END
	                              END
	                          END
	                      END
	                  END
	                  
					  ||'               
	                  </center>' as acao,
		              CASE WHEN item.ipastatus = 'A' THEN 'Aprovado' 
					      ELSE 
					  	  CASE WHEN item.ipastatus = 'C' THEN 'Cancelado'
					  	      ELSE
					  		  CASE WHEN item.ipastatus = 'F' THEN 'Faturado'
							      ELSE
								  CASE WHEN item.ipastatus = 'P' THEN 'Pago'
								      ELSE
									  CASE WHEN item.ipastatus = 'G' THEN 'Glosado'
									  END
							      END
					  		  END
					      END	
		              END AS situacao,
		               CASE 
						    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 10 ) THEN
							
						        substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '000' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
						    ELSE
							CASE 
							    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 100 ) THEN
						
								substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '00' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
							    ELSE
								CASE 
								    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 1000 ) THEN
						
									substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '0' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
									
								END
							END
							
						END as ipanumitempad,
		              fornec.fornome,
		              tipo.tsedsc,
		              item.ipaqtd||' ',
		              item.ipavalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.glosaitempad glo WHERE glo.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(glosa.gipvalor AS text) END AS gipvalor,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(fatura.fipdataftura,'DD/MM/YYYY') END AS fipdataftura,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE fatura.fipnumfaturaagencia||' ' END AS fipnumfaturaagencia,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(fatura.fipvalortotal AS text) END AS fipvalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(pag.pipdatapagamento,'DD/MM/YYYY') END AS pipdatapagamento,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipnumordembancaria||' ' AS text) END AS pipnumordembancaria,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipvalorfaturado AS text) END AS pipvalorfaturado
		              
	                  FROM $this->stNomeTabela item
	                  JOIN publicidade.pad pad ON item.padid= pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid and fatura.fipstatus = 'A'
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid and pag.pipstatus = 'A'
	                  {$where} 
	                  ORDER BY ipanumitempad ";
            }

            $cabecalho = array('A&ccedil;&atilde;o', 'Status', 'N&uacute;mero', 'Fornecedor', 'Tipo de Servi&ccedil;o', 'Qtd.', 'Valor Total', 'Valor Glosado', 'Dt. Faturamento', 'N&ordm; Nota', 'Valor Faturado', 'Dt. Pagamento', 'N&ordm; da OB', 'Valor Pago');
            $this->monta_lista($sql, $cabecalho, 10, 50, false, "center", 'S', 'formularioItens', '', array('left', 'center', 'right'));
        }
    }

    /**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $ipaid - Identificador do registro
     * @return array
     */
    public function carrega_registro_por_id($ipaid) {
        if (!empty($ipaid)) {
            $ipaid = pg_escape_string($ipaid);
            $sql = "SELECT item.ipaid,
						   item.tseid,
						   item.catid,
						   item.padid,
						   item.tppid,
						   item.ipanumitempad,
						   item.ipaqtd,
						   item.ipavalorservico,
						   item.ipahonorario,
						   item.ipavalorhonorario,
						   item.ipastatus,
						   item.forid,
						   item.ipavalortotal,
						   item.ipadsc,
						   item.ipadesconto,
						   item.ipavaloritem,
						   tipo.tsedsc
						   
						   FROM $this->stNomeTabela item
						   JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid	
						   WHERE item.ipaid='{$ipaid}' ";

            return $this->pegaLinha($sql);
        }
    }

    /**
     * Pega o saldo atual da PAD
     * @name pegar_saldo_pad
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro
     * @param string $padvalor - Valor da PAD
     * @return float
     */
    public function pegar_saldo_pad($padid, $padvalor) {
        if (!empty($padvalor) && !empty($padid)) {
            $padid = pg_escape_string($padid);
            $padvalor = pg_escape_string($padvalor);

            $totalUsado = $this->pegar_valor_usado($padid);

            if (strpos($padvalor, '.') !== false) {
                $padvalor = str_replace('.', '', $padvalor);
            }

            $subtrai = new Math($padvalor, $totalUsado, 2);
            $saldo = $subtrai->sub()->getResult();

            return number_format($saldo, 2, ',', '.');
        }
    }

    /**
     * Pega o valor já utilizado 
     * @name pegar_valor_usado
     * @author Alysson Rafael
     * @access public
     * @param int $padid - Identificador do registro
     * @return float
     */
    public function pegar_valor_usado($padid) {
        if (!empty($padid)) {
            $padid = pg_escape_string($padid);
            $sql = "SELECT SUM(ipavalortotal) FROM $this->stNomeTabela WHERE padid='{$padid}' AND ipastatus <> 'C' ";
            $resultado = $this->pegaUm($sql);
            return $resultado;
        }
    }

    /**
     * Verifica se o saldo da PAD é suficiente ao inserir item
     * @name verifica_saldo_disponivel
     * @author Alysson Rafael
     * @access public
     * @param array $post - Array com os dados
     * @return bool
     */
    public function verifica_saldo_disponivel($post) {
        if (!empty($post['saldo'])) {
            $ipavalortotal = pg_escape_string($post['ipavalortotal']);
            $padvalor = pg_escape_string($post['padvalor']);
            $saldo = pg_escape_string($post['saldo']);
            $erro = false;

            $saldo = str_replace('.', '', $saldo);
            $ipavalortotal = str_replace('.', '', $ipavalortotal);

            /* $soma = new Math($saldo,$ipavalortotal,2);
              $totalPretendido = $soma->sum()->getResult();

              if(!empty($post['ipaid'])){
              $itempad = $this->carrega_registro_por_id($post['ipaid']);

              $subtrai = new Math($totalPretendido, $itempad['ipavalortotal'],2);
              $totalPretendido = $subtrai->sub()->getResult();
              }

              $validacao = new Math($saldo,$totalPretendido,2); */

            $validacao = new Math($saldo, $ipavalortotal, 2);

            if ($validacao->isLess()) {
                $erro = true;
            }

            return $erro;
        }
    }

    /**
     * Atualizar status de item da PAD
     * @name atualizar_status
     * @author Alysson Rafael
     * @access public
     * @param int $ipaid - Identificador do registro
     * @param string $ipastatus - Status a ser atualizado
     * @return void
     */
    public function atualizar_status($ipaid, $ipastatus) {
        if (!empty($ipastatus) && !empty($ipaid)) {
            $ipastatus = pg_escape_string($ipastatus);
            $ipaid = pg_escape_string($ipaid);
            $sql = "UPDATE $this->stNomeTabela SET ipastatus='{$ipastatus}' WHERE ipaid='{$ipaid}' ";
            $this->executar($sql);
            $this->commit();
        }
    }

    /**
     * Verifica se o item da pad está com status APROVADO
     * 
     * @name isStatusAprovado
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
    public function isStatusAprovado() {
        return $this->ipastatus == self::ST_APROVADO;
    }

    /**
     * Verifica se o item da pad está com status FATURADO
     * 
     * @name isStatusFaturado
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
    public function isStatusFaturado() {
        return $this->ipastatus == self::ST_FATURADO;
    }

    /**
     * Verifica se o item da pad está com status GLOSADO
     * 
     * @name isStatusGlosado
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
    public function isStatusGlosado() {
        return $this->ipastatus == self::ST_GLOSADO;
    }

    /**
     * Verifica se o item da pad está com status PAGO
     * 
     * @name isStatusPago
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
    public function isStatusPago() {
        return $this->ipastatus == self::ST_PAGO;
    }

    /**
     * Verifica se o item da pad está com status CANCELADO
     * 
     * @name isStatusCancelado
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
    public function isStatusCancelado() {
        return $this->ipastatus == self::ST_CANCELADO;
    }

    /**
     * Listagem dos dados para relatório(memorando)
     * @name listar_por_pad_memorando
     * @author Alysson Rafael
     * @param string $padnumero - Número da PAD
     * @access public
     * @return void
     */
    public function listar_por_pad_memorando($padnumero) {

        if (!empty($padnumero)) {
            // Valida se existem itens marcados de outras páginas
            if (count($_SESSION['ITENS_MARCADOS'])) {

                $arrItem = $_SESSION['ITENS_MARCADOS'];
                $arrItem = implode(",", $arrItem);

                $checkBox = "
				'<center>' ||
					CASE 
						WHEN 
							item.ipaid in ( {$arrItem} )
						THEN 
							'<input type=\"checkbox\" checked name=\"checkitempad\" id=\"checkitempad\" value=\"'||item.ipaid||'\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\">'
						ELSE
							'<input type=\"checkbox\" name=\"checkitempad\" id=\"checkitempad\" value=\"'||item.ipaid||'\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\">'
					END
				|| '</center>'
				";
            } else {
                $checkBox = "'<center><input type=\"checkbox\" name=\"checkitempad\" id=\"checkitempad\" value=\"'||item.ipaid||'\" onclick=\"javascript:habilitaImpressao( this.value , this.checked, '||pad.padid||' );\"></center>'";
            }


            $padnumero = str_replace('/', '', pg_escape_string($padnumero));

            $where = " WHERE pad.padnumero='{$padnumero}' AND item.ipastatus in ('F', 'G') ";

            $sql = "SELECT 
					  {$checkBox}        
	                  as acao,
		              item.ipanumitempad,
		              fornec.fornome,
		              tipo.tsedsc,
		              item.ipaqtd,
		              item.ipavalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.glosaitempad glo WHERE glo.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(glosa.gipvalor AS text) END AS gipvalor,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(fatura.fipdataftura,'DD/MM/YYYY') END AS fipdataftura,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE fatura.fipnumfaturaagencia  || ' ' END AS fipnumfaturaagencia,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(fatura.fipvalortotal AS text) END AS fipvalortotal
		              
	                  FROM $this->stNomeTabela item
	                  JOIN publicidade.pad pad ON item.padid=pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid
	                  {$where} 
	                  ORDER BY item.ipanumitempad ";

            $cabecalho = array('<input type="checkbox" id="action_toogleCheckboxes" onclick="javascript:toogleCheckboxes();"> A&ccedil;&atilde;o',
                'N&uacute;mero', 'Fornecedor', 'Tipo de Servi&ccedil;o', 'Qtd.',
                'Valor Total', 'Valor Glosado', 'Dt. Faturamento', 'N&ordm; Nota', 'Valor Faturado');
            $this->monta_lista($sql, $cabecalho, 10, 50, false, "center", 'S', 'formularioItens', '', array('left', 'center', 'right'), '', array('classTable' => 'lista_itens_pad'));
        }
    }

    /**
     * Conta os itens da pad com estado faturado
     * @name conta_itens
     * @author Alysson Rafael
     * @access public
     * @param string $padnumero - Número da PAD
     * @return int
     */
    public function conta_itens($padnumero) {
        if (!empty($padnumero)) {
            $padnumero = str_replace('/', '', pg_escape_string($padnumero));

            $where = " WHERE pad.padnumero='{$padnumero}' AND item.ipastatus in ('F','G') ";

            $sql = "SELECT 
					  COUNT(*) AS qtd
		              
	                  FROM $this->stNomeTabela item
	                  JOIN publicidade.pad pad ON item.padid=pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid
	                  {$where} 
	                   ";
            $qtd = $this->pegaUm($sql);

            return $qtd;
        }
    }

    /**
     * Pega dados para o relatório memorando
     * @name pegar_dados_memorando
     * @author Alysson Rafael
     * @access public
     * @param array $ipaids - Array com os ids dos itens da pad
     * @return array
     */
    public function pegar_dados_memorando($ipaids) {
        if (is_array($ipaids)) {
            foreach ($ipaids as $key => $value) {
                if (!is_numeric($value)) {
                    unset($ipaids[$key]);
                }
            }
        }
        //verifica se existem checkboxes marcados em outra páginas
        if (count($_SESSION['ITENS_MARCADOS'])) {
            // Carrega array 
            $arrItem = $_SESSION['ITENS_MARCADOS'];
            $arrItem = implode(",", $arrItem);


            if (count($ipaids) >= 1) {
                $stripaids = $arrItem . ', ' . pg_escape_string(implode(',', $ipaids));
            } else {
                $stripaids = $arrItem;
            }
        } else {
            if (count($ipaids) >= 1) {
                $stripaids = pg_escape_string(implode(',', $ipaids));
            }
        }

        if ((count($ipaids) >= 1) || (count($_SESSION['ITENS_MARCADOS']))) {

            $sql = "SELECT 
					fatura.fipnumfaturaagencia,
					fatura.fipvalortotal,
					fatura.fipptres
					FROM $this->stNomeTabela item
					JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
					WHERE item.ipaid IN (" . $stripaids . ") ";
            //var_dump((string) $sql);exit;   
            $resultado = $this->carregar($sql);
            return $resultado;
        }
    }

    /**
     * Pega total de itens aprovados, faturados e pagos
     * @name pegar_valores_execucao_fiscal
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
    public function pegar_valores_execucao_fiscal($get) {

        if (!empty($get['datainicial']) && !empty($get['datafinal']) && !empty($get['agencias'])) {
            $get['datainicial'] = formata_data_sql(pg_escape_string($get['datainicial']));
            $get['datafinal'] = formata_data_sql(pg_escape_string($get['datafinal']));
            $get['ano'] = pg_escape_string($get['ano']);


            //explode a string com os ids das agências
            $aux = explode(',', $get['agencias']);
            $retorno = array();
            if (is_array($aux) && count($aux) >= 1) {

                foreach ($aux as $key => $value) {
                    $value = pg_escape_string($value);
                    $sql = "SELECT SUM(item.ipavalortotal) AS valorAprovado
							FROM publicidade.itenspad item 
							JOIN publicidade.pad pad ON item.padid=pad.padid
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
							WHERE cont.forid='{$value}'
							AND (item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus='A')
							AND cont.cttstatus='A'
                                                       	AND pad.padstatus='A' ";

                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }

                    $valorAprovado = $this->pegaUm($sql);
                    if (is_null($valorAprovado)) {
                        $valorAprovado = 0;
                    }

                    $sql = "SELECT SUM(fatura.fipvalortotal) AS valorFaturado
							FROM publicidade.itenspad item 
							JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
							JOIN publicidade.pad pad ON item.padid=pad.padid
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
							WHERE cont.forid='{$value}'
							AND (fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A')
							AND cont.cttstatus='A'
							AND item.ipastatus='F'
                                                        AND pad.padstatus='A' ";
                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }
                    $valorFaturado = $this->pegaUm($sql);
                    if (is_null($valorFaturado)) {
                        $valorFaturado = 0;
                    }

                    $sql = "SELECT SUM(fatura.fipvalortotal) AS valorFaturadoComGlosa
					        FROM publicidade.itenspad item
					        JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
					        JOIN publicidade.pad pad ON item.padid=pad.padid
					        JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
                                                join publicidade.glosaitempad glo on glo.ipaid = item.ipaid
					        WHERE cont.forid='{$value}'
					        AND (fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A')
					        AND cont.cttstatus='A'
					        AND item.ipastatus='G'
                                                AND pad.padstatus='A' ";
                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }

                    $valorFaturadoComGlosa = $this->pegaUm($sql);
                    if (is_null($valorFaturadoComGlosa)) {
                        $valorFaturadoComGlosa = 0;
                    }


                    $sql = "SELECT item.ipaid
					        FROM publicidade.itenspad item
					        JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
					        JOIN publicidade.pad pad ON item.padid=pad.padid
					        JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
					        WHERE cont.forid='{$value}'
					        AND (fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A')
					        AND cont.cttstatus='A'
					        AND item.ipastatus='G'
                                                AND pad.padstatus='A' ";
                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }
                    $result = $this->carregar($sql);

                    $ids = '';
                    if (is_array($result) && count($result)) {
                        foreach ($result as $key2 => $value2) {
                            $ids .= $value2['ipaid'] . ',';
                        }
                        $ids = substr($ids, 0, strrpos($ids, ','));
                    }


                    $sql = "SELECT SUM(item.ipavalortotal) AS valorAprovadoComGlosa
					        FROM publicidade.itenspad item
					        JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
					        JOIN publicidade.pad pad ON item.padid=pad.padid
					        JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
					        WHERE cont.forid='{$value}'
					        AND (item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND fatura.fipstatus = 'A')
					        AND cont.cttstatus='A'
					        AND item.ipastatus='G'
                                                AND pad.padstatus='A' ";
                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }
                    if (!empty($ids)) {
                        $sql .= "AND item.ipaid NOT IN ({$ids}) ";
                    }


                    $valorAprovadoComGlosa = $this->pegaUm($sql);
                    if (is_null($valorAprovadoComGlosa)) {
                        $valorAprovadoComGlosa = 0;
                    }

                    $sql = "SELECT SUM(CAST(pgto.pipvalorfaturado AS float)) AS valorPago
							FROM publicidade.itenspad item 
							JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
							JOIN publicidade.pad pad ON item.padid=pad.padid
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid
							WHERE cont.forid='{$value}'
							AND (pgto.pipdatapagamento >= '{$get['datainicial']}' AND pgto.pipdatapagamento <= '{$get['datafinal']}' AND pgto.pipstatus = 'A')
							AND cont.cttstatus='A'
							AND item.ipastatus='P'
                                                        AND pad.padstatus='A' ";
                    if ($get['ano'] != 'x') {
                        $sql .= " and pad.padano = " . $get['ano'];
                    }


                    $valorPago = $this->pegaUm($sql);
                    if (is_null($valorPago)) {
                        $valorPago = 0;
                    }
                    $retorno[$value] = array('valorAprovado' => $valorAprovado, 'valorFaturado' => $valorFaturado, 'valorPago' => $valorPago, 'valorFaturadoComGlosa' => $valorFaturadoComGlosa, 'valorAprovadoComGlosa' => $valorAprovadoComGlosa);
                }
            }


            return $retorno;
        }
    }

    /**
     * Pega total de itens retroativamente, trazendo um item como aprovado mesmo que
     * este esteja faturado, desde que o período informado abranja a data de cadastro
     * do item mas não abranja a data de faturamento. Mesma coisa com os que já estiverem
     * pagos mas o filtro abranja apenas a data de faturamento e não a de pgto, ai traz ele como faturado.
     * @name pegar_valores_execucao_fiscal_retroativo
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
    public function pegar_valores_execucao_fiscal_retroativo($get) {
        if (!empty($get['datainicial']) && !empty($get['datafinal']) && !empty($get['agencias'])) {
            $get['datainicial'] = formata_data_sql(pg_escape_string($get['datainicial']));
            $get['datafinal'] = formata_data_sql(pg_escape_string($get['datafinal']));

            $aux = explode(',', $get['agencias']);
            $retorno = array();

            if (is_array($aux) && count($aux) >= 1) {
                foreach ($aux as $key => $value) {
                    $value = pg_escape_string($value);

                    $sql = "SELECT 
							SUM(item.ipavalortotal) AS valorAprovado 
							FROM publicidade.itenspad item 
							JOIN publicidade.faturamentoitempad fat ON item.ipaid=fat.ipaid
							JOIN publicidade.pad pad ON item.padid=pad.padid 
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid 
							WHERE cont.forid='{$value}' 
							AND (item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}' AND item.ipastatus='F') 
							AND cont.cttstatus='A' 
							AND pad.padstatus='A' 
							AND((fat.fipdataftura<'{$get['datainicial']}' OR fat.fipdataftura>'{$get['datafinal']}') AND fat.fipstatus='A' ) ";

                    $valorAprovado = $this->pegaUm($sql);
                    if (is_null($valorAprovado)) {
                        $valorAprovado = 0;
                    }

                    $sql = "SELECT 
							SUM(item.ipavalortotal) AS valorFaturado 
							FROM publicidade.itenspad item 
							JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid 
							JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
							JOIN publicidade.pad pad ON item.padid=pad.padid 
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid 
							WHERE cont.forid='{$value}' 
							AND (fatura.fipdataftura >= '{$get['datainicial']}' AND fatura.fipdataftura <= '{$get['datafinal']}' AND fatura.fipstatus = 'A')
							AND ((pgto.pipdatapagamento < '{$get['datainicial']}' OR pgto.pipdatapagamento > '{$get['datafinal']}') AND pgto.pipstatus='A') 
							AND cont.cttstatus='A' 
							AND item.ipastatus='P' 
							AND pad.padstatus='A' ";

                    $valorFaturado = $this->pegaUm($sql);
                    if (is_null($valorFaturado)) {
                        $valorFaturado = 0;
                    }

                    $sql = "SELECT 
							SUM(item.ipavalortotal) AS valorAprovado 
							FROM publicidade.itenspad item 
							JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid 
							JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
							JOIN publicidade.pad pad ON item.padid=pad.padid 
							JOIN publicidade.contrato cont ON pad.cttid=cont.cttid 
							WHERE cont.forid='{$value}' 
							AND (item.ipadata >= '{$get['datainicial']}' AND item.ipadata <= '{$get['datafinal']}')
							AND ((fatura.fipdataftura < '{$get['datainicial']}' OR fatura.fipdataftura > '{$get['datafinal']}') AND fatura.fipstatus = 'A')
							AND ((pgto.pipdatapagamento < '{$get['datainicial']}' OR pgto.pipdatapagamento > '{$get['datafinal']}') AND pgto.pipstatus='A') 
							AND cont.cttstatus='A' 
							AND pad.padstatus='A' ";

                    $valorAprovado2 = $this->pegaUm($sql);
                    if (is_null($valorAprovado2)) {
                        $valorAprovado2 = 0;
                    }
                    $soma = new Math($valorAprovado, $valorAprovado2, 2);
                    $valorAprovado = $soma->sum()->getResult();

                    $retorno[$value] = array('valorAprovado' => $valorAprovado, 'valorFaturado' => $valorFaturado);
                }
            }

            return $retorno;
        }
    }

    /**
     * Soma os valores dos itens
     * @name pega_somatorio_valores
     * @author Alysson Rafael
     * @access public
     * @param string $ipaids - String com os ids dos itens que terão o valor somado
     * @return array
     */
    public function pega_somatorio_valores($ipaids) {
        if (!empty($ipaids)) {
            $ipaids = pg_escape_string($ipaids);
            $sql = "SELECT SUM(ipavalorservico) AS ipavalorservico,SUM(ipavalortotal) AS ipavalortotal ";
            $sql .= "FROM $this->stNomeTabela WHERE ipaid IN (" . $ipaids . ") ";
            $valores = $this->carregar($sql);
            return $valores[0];
        }
    }

    /**
     * Pega dados para o relatório espelho da pad
     * @name pega_itens_por_pad_espelho_pad
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
    public function pega_itens_por_pad_espelho_pad($get) {

        //verifica se existem checkboxes marcados em outra páginas
        if (count($_SESSION['ITENS_MARCADOS_LIST'])) {
            // Carrega array 
            $arrItem = $_SESSION['ITENS_MARCADOS_LIST'];
            $arrItem = implode(",", $arrItem);


            if (count($ipaids) >= 1) {
                $stripaids = $arrItem . ', ' . pg_escape_string(implode(',', $ipaids));
            } else {
                $stripaids = $arrItem;
            }
        } else {
            if (count($ipaids) >= 1) {
                $stripaids = pg_escape_string(implode(',', $ipaids));
            }
        }





        if (!empty($get['padid'])) {
            $get['padid'] = pg_escape_string($get['padid']);
            $sql = "SELECT item.ipanumitempad AS ipanumitempad,
						   item.ipadsc AS ipadsc,
						   fornec.fornome AS fornome,
						   
						   CASE WHEN
						       (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1
						       THEN '-'
						       ELSE SUBSTR(fatura.fipptres,1,2)||'.'||SUBSTR(fatura.fipptres,3,3)||'.'||SUBSTR(fatura.fipptres,6,4)||'.'||SUBSTR(fatura.fipptres,10,4)||'.'||SUBSTR(fatura.fipptres,14,4) 
						   END AS fipptres,
						   
						   item.ipavalortotal AS ipavalortotal,
						   
						   CASE WHEN
						       (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 
						       THEN '-'
						       ELSE fatura.fipnumfaturaagencia  	
						   END AS fipnumfaturaagencia,
						   
						   CASE WHEN
						       (SELECT COUNT(*) FROM publicidade.pagamentoitempad pg WHERE pg.ipaid=item.ipaid) < 1
						       THEN '-'
						       ELSE pgto.pipnumordembancaria
						   END AS pipnumordembancaria
						   FROM publicidade.fornecedor fornec
						   JOIN $this->stNomeTabela item ON fornec.forid=item.forid
						   LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid
						   LEFT JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
						   WHERE item.padid='{$get['padid']}'";

            if (count($_SESSION['ITENS_MARCADOS_LIST'])) {
                $sql .= "AND
							item.ipaid IN (" . $stripaids . ")
						";
            }

            $sql .=
                    "
						   ORDER BY item.ipanumitempad ";

            $resultado = $this->carregar($sql);
            return $resultado;
        }
    }

    /**
     * Pega a soma do valor total para o relatório espelho da pad
     * @name pega_qtd_itens_por_pad_espelho_pad
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return float
     */
    public function pega_qtd_itens_por_pad_espelho_pad($get) {
        if (!empty($get['padid'])) {
            $get['padid'] = pg_escape_string($get['padid']);
            $sql = "SELECT 
					SUM(item.ipavalortotal) AS tot
					FROM publicidade.fornecedor fornec 
					JOIN publicidade.itenspad item ON fornec.forid=item.forid 
					WHERE item.padid='{$get['padid']}' ";

            return $this->pegaUm($sql);
        }
    }

    /**
     * Atualiza os valores do item de acordo com o valor da glosa
     * 
     * @name atualizar_valores_item
     * @author Alysson Rafael
     * @param int $ipaid      - Identificador do registro
     * @param float $gipvalor - Valor glosado
     * @param int $qtdItens   - Qtd de itens pela qual será dividido o valor da glosa
     * @access public
     * @return void
     */
    public function atualizar_valores_item($ipaid, $gipvalor, $qtdItens) {
        if (!empty($ipaid) && !empty($gipvalor) && $qtdItens >= 1) {
            $ipaid = pg_escape_string($ipaid);

            $gipvalor = str_replace('.', '', $gipvalor); // edit

            $divide = new Math($gipvalor, $qtdItens, 2);
            $gipvalorDividido = $divide->div()->getResult();

            //pega os valores da fatura do item
            $dadosItem = $this->carrega_registro_por_id($ipaid);

            if (is_array($dadosItem) && count($dadosItem) >= 1) {

                if ($dadosItem['tsedsc'] == 'Produção') {

                    $subtracao = new Math($dadosItem['ipavalorservico'], $gipvalorDividido, 2);
                    $dadosItem['ipavalorservico'] = $subtracao->sub()->getResult();

                    //calcula valor do honorário
                    $multiplica = new Math($dadosItem['ipavalorservico'], $dadosItem['ipahonorario'], 2);
                    $parcial = $multiplica->mul()->getResult();
                    $divide = new Math($parcial, 100, 2);
                    $dadosItem['ipavalorhonorario'] = $divide->div()->getResult();

                    //calcula valor do item, valor total e valor desconto
                    $soma = new Math($dadosItem['ipavalorservico'], $dadosItem['ipavalorhonorario'], 2);
                    $dadosItem['ipavaloritem'] = $soma->sum()->getResult();
                    $dadosItem['ipavalortotal'] = $dadosItem['ipavaloritem'];
                    $dadosItem['ipadesconto'] = '0,00';
                } else if ($dadosItem['tsedsc'] == 'Mídia') {

                    $subtracao = new Math($dadosItem['ipavaloritem'], $gipvalorDividido, 2);
                    $dadosItem['ipavaloritem'] = $subtracao->sub()->getResult();

                    //calcula o valor do desconto
                    $multiplica = new Math($dadosItem['ipavaloritem'], 5, 2);
                    $parcial = $multiplica->mul()->getResult();
                    $divide = new Math($parcial, 100, 2);
                    $dadosItem['ipadesconto'] = $divide->div()->getResult();

                    //calcula o valor do honorário
                    $multiplica = new Math($dadosItem['ipavaloritem'], 15, 2);
                    $parcial = $multiplica->mul()->getResult();
                    $divide = new Math($parcial, 100, 2);
                    $dadosItem['ipavalorhonorario'] = $divide->div()->getResult();

                    //calcula o valor do serviço
                    $multiplica = new Math($dadosItem['ipavaloritem'], 80, 2);
                    $parcial = $multiplica->mul()->getResult();
                    $divide = new Math($parcial, 100, 2);
                    $dadosItem['ipavalorservico'] = $divide->div()->getResult();

                    //calcula o valor total
                    $soma = new Math($dadosItem['ipavalorservico'], $dadosItem['ipavalorhonorario'], 2);
                    $dadosItem['ipavalortotal'] = $soma->sum()->getResult();
                }
                $dadosItem['id'] = $dadosItem['padid'];
                $dadosItem['ipavalorservico'] = formata_valor($dadosItem['ipavalorservico']);
                $dadosItem['ipavalorhonorario'] = formata_valor($dadosItem['ipavalorhonorario']);
                $dadosItem['ipavaloritem'] = formata_valor($dadosItem['ipavaloritem']);
                $dadosItem['ipavalortotal'] = formata_valor($dadosItem['ipavalortotal']);
                $dadosItem['ipadesconto'] = formata_valor($dadosItem['ipadesconto']);

                $this->salvar($dadosItem);
            }
        }
    }

}
