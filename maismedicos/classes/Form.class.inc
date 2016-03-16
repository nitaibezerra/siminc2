<?php
class Form extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(  );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array();
    
    public function recuperaPerguntas($id = null, $idGrupo = null)
    {
    	if($idGrupo){
    		$arWhere[] = "p.pergrupo = {$idGrupo}";
    	}
    	$sql = "select p.* from maismedicos.form f
				join maismedicos.form_pergunta p on f.fomid = p.fomid
				where f.fomstatus = 'A' 
				and p.perstatus = 'A'
				and f.fomnome = 'Relatório de Supervisão Periódica'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
				order by pergrupo, perordem";
    	//var_dump($this->carregar($sql)); die;
    	return $this->carregar($sql);
    }
    
    public function recuperaRespostas($id = null, $idGrupo = null, $data = array())
    {
    	if($idGrupo){
    		$arWhere[] = "p.pergrupo = {$idGrupo}";
    	}
    	
    	/*
    	$perfilSup = $this->pegaUm("(select pflcod from seguranca.perfil where  pfldsc = 'Supervisor Grupo Especial' and sisid = (select sisid from seguranca.sistema  where sisdiretorio = 'maismedicos'))");
    	if(in_array($perfilSup, arrayPerfil())){
    		
    	}
    	*/
    	
    	if($_REQUEST['resid']){
    		$arWhere[] = "r.resid = {$_REQUEST['resid']}";
    	}else{
    		$arWhere[] = "r.usucpf = '{$_SESSION['usucpf']}'";
    		$arWhere[] = "r.resdtenvio is null";
    	}
    	
    	
    	$sql = "select p.*, r.*, i.* from maismedicos.form f
				join maismedicos.form_pergunta p on f.fomid = p.fomid
    			join maismedicos.form_resposta r on f.fomid = r.fomid
    			join maismedicos.form_resposta_item i on i.perid = p.perid
				where f.fomstatus = 'A' 
				and p.perstatus = 'A'
				and f.fomnome = 'Relatório de Supervisão Periódica'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
				order by r.resid desc, pergrupo, perordem";
    	
    	return $this->carregar($sql);
    }
    
    function recuperaRelatorios($id = null)
    {
    	$sql = "select distinct
    				case when resdtenvio is null then 
	    			'<img src=\"../imagens/alterar.gif\" id=\"' || r.resid || '\" class=\"btnEdita\" style=\"cursor:pointer\" />'
    				else
    				'<img src=\"../imagens/consultar.gif\" id=\"' || r.resid || '\" class=\"btnVisualiza\" style=\"cursor:pointer\" />'
    				end as acao,
    				--i.reidsc,
	    			to_char(r.resdtcadastro, 'DD/MM/YYYY') as dtcadastro, 
	    			to_char(r.resdtatualiza, 'DD/MM/YYYY') as dtatualiza, 
	    			to_char(r.resdtenvio, 'DD/MM/YYYY') as dtenvio
    			from maismedicos.form f
				--join maismedicos.form_pergunta p on f.fomid = p.fomid
    			--	and p.pernome = 'Em qual mês foi realizada esta supervisão?'
    			--	and p.perstatus = 'A'
    			left join maismedicos.form_resposta r on f.fomid = r.fomid
    				and r.resstatus = 'A'
    			--left join maismedicos.form_resposta_item i on i.perid = p.perid
    			--	and i.reistatus = 'A'
				where f.fomstatus = 'A'				
				and f.fomnome = 'Relatório de Supervisão Periódica'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
				--order by r.resid desc, pergrupo, perordem";
    	
    	
    	$arCabecalho = array('Ação','Data Cadastro','Data Última Atualização','Data Envio');
    	return $this->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', '');
    }
    
    function recuperaRelatoriosCaixaEntrada($id = null)
    {
    	$sql = "select distinct
    				case when resdtenvio is null then
	    			'<img src=\"../imagens/alterar.gif\" id=\"' || r.resid || '\" class=\"btnEdita\" style=\"cursor:pointer\" />'
    				else
    				'<img src=\"../imagens/consultar.gif\" id=\"' || r.resid || '\" class=\"btnVisualiza\" style=\"cursor:pointer\" />'
    				end as acao,
    				--i.reidsc,
	    			to_char(r.resdtcadastro, 'DD/MM/YYYY') as dtcadastro,
	    			to_char(r.resdtatualiza, 'DD/MM/YYYY') as dtatualiza,
	    			to_char(r.resdtenvio, 'DD/MM/YYYY') as dtenvio
    			from maismedicos.form f
				--join maismedicos.form_pergunta p on f.fomid = p.fomid
    			--	and p.pernome = 'Em qual mês foi realizada esta supervisão?'
    			--	and p.perstatus = 'A'
    			left join maismedicos.form_resposta r on f.fomid = r.fomid
    				and r.resstatus = 'A'
    			--left join maismedicos.form_resposta_item i on i.perid = p.perid
    			--	and i.reistatus = 'A'
				where f.fomstatus = 'A'
				and f.fomnome = 'Relatório de Supervisão Periódica'
    			and resdtenvio is not null
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
				--order by r.resid desc, pergrupo, perordem";
    	 
    	 
    	$arCabecalho = array('Ação','Data Cadastro','Data Última Atualização','Data Envio');
    	return $this->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', '');
    }
    
    public function montaForm($id = null)
    {
    	$rs = $this->recuperaPerguntas($id);
    	$resp = $this->recuperaRespostas($id);
    	
    	if($rs){
    		if($resp){
    			foreach($resp as $k => $r){
    				$resid = $resid ? $resid : $r['resid'];
    				if(in_array(trim($r['pertipo']), array('checkbox', 'radio'))){
	    				$arResposta[$r['perid']]['itmid'][] = trim($r['itmid']);
	    				$arResposta[$r['perid']]['reidsc'][$r['itmid']] = trim($r['reidsc']);
    				}else 
    				if(in_array(trim($r['pertipo']), array('grid'))){
    					$arResposta[$r['perid']]['itmid'][$r['itmid']] = trim($r['itmid']);
    					$arResposta[$r['perid']]['reidsc'][$r['itmid']] = trim($r['reidsc']);
    				}else{
	    				$arResposta[$r['perid']]['itmid'] = trim($r['itmid']);
	    				$arResposta[$r['perid']]['reidsc'] =trim($r['reidsc']);    					
    				}
    			}
    		}
    		foreach($rs as $f){
    			$fomid = $f['fomid'];
    			$grupos[$f['pergrupo']] = $f['pergrupo'];
    			$dados[$f['pergrupo']][$f['perid']]['grupo'] = $f['pergrupo'];
    			$dados[$f['pergrupo']][$f['perid']]['id'] = $f['perid'];
    			$dados[$f['pergrupo']][$f['perid']]['nome'] = $f['pernome'];
    			$dados[$f['pergrupo']][$f['perid']]['ajuda'] = $f['perajuda'];
    			$dados[$f['pergrupo']][$f['perid']]['tipo'] = $f['pertipo'];
    			$dados[$f['pergrupo']][$f['perid']]['obrigatorio'] = $f['perobrigatorio'];
    			$dados[$f['pergrupo']][$f['perid']]['itmid'] = $arResposta[$f['perid']]['itmid'];
    			$dados[$f['pergrupo']][$f['perid']]['reidsc'] = $arResposta[$f['perid']]['reidsc'];
    			$dados[$f['pergrupo']][$f['perid']]['class'] = $f['perobrigatorio'] ? $f['perclasse'].' required' : $f['perclasse'];																;
    		}
    		if($dados){
    			$x=0;
    			$html = '';
    			foreach($grupos as $grupo){
    				$x++;
    				$html .= '<form name="formulario'.$grupo.'" id="formulario'.$grupo.'" method="post" action="">';
    				$html .= '<input type="hidden" name="fomid" id="fomid" value="'.$fomid.'" />';
    				$html .= '<input type="hidden" name="resid" id="resid" value="'.$resid.'" />';
    				$html .= '<input type="hidden" name="pergrupo" id="pergrupo" value="'.$grupo.'" />';
    				$html .= '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">';
    				foreach($dados[$grupo] as $item){
    					if($item['tipo']=='titulo'){
    						$html .= '<tr><td colspan="2" class="subtitulocentro"><span class="'.$item['class'].'"><font size="+1">'.$item['nome'].'</font></span></td></tr>';
    					}else
    					if($item['tipo']=='subtitulo'){
    						$html .= '<tr><td colspan="2" class="subtitulocentro"><span class="'.$item['class'].'">'.$item['nome'].'</span></td></tr>';
    					}else
    					if($item['tipo']=='grid'){
    						$html .= '<tr><td colspan="2" align="center">'.$this->montaCampo($item).'</td></tr>';
    					}else{
    						$html .= '
    								<tr>
										<td width="50%" class="subtitulodireita">
    									'.$item['nome'].'<br/>
    									<font class="txtajuda">'.$item['ajuda'].'</font>
    									</td>
										<td>'.$this->montaCampo($item).'</td>
									</tr>
    								';
    					}
    				}
    				$html .= '<tr><td colspan="2" algin="center">
    						<center>
    						'.($x>1 ? '<input type="button" value="Anterior" class="btnSalvar btnAnterior" id="btnAnterior_'.($grupo-1).'" />' : '').'
							<input type="button" value="Salvar" class="btnSalvar" id="btnSalvar_'.$grupo.'" />
							'.($x==count($grupos) ? '<input type="button" value="Enviar" class="btnEnviar" id="btnEnviar_'.$grupo.'" />' : '').'
							'.($x<count($grupos) ? '<input type="button" value="Próximo" class="btnSalvar btnProximo" id="btnProximo_'.($grupo+1).'" />' : '').'
							</center>
							</td></tr>
    						</table><p>&nbsp;</p>';
    				$html .= '</form>';
    			}
    		}
    	}
    	echo $html;
    }
    public function montaRelatorioRespostas($id = null)
    {
    	$rs = $this->recuperaPerguntas($id);
    	$resp = $this->recuperaRespostas($id);
    	
    	if($rs){
    		if($resp){
    			foreach($resp as $k => $r){
    				$resid = $resid ? $resid : $r['resid'];
    				if(in_array(trim($r['pertipo']), array('checkbox', 'radio'))){
	    				$arResposta[$r['perid']]['itmid'][] = trim($r['itmid']);
	    				$arResposta[$r['perid']]['reidsc'][$r['itmid']] = trim($r['reidsc']);
    				}else 
    				if(in_array(trim($r['pertipo']), array('grid'))){
    					$arResposta[$r['perid']]['itmid'][$r['itmid']] = trim($r['itmid']);
    					$arResposta[$r['perid']]['reidsc'][$r['itmid']] = trim($r['reidsc']);
    				}else{
	    				$arResposta[$r['perid']]['itmid'] = trim($r['itmid']);
	    				$arResposta[$r['perid']]['reidsc'] =trim($r['reidsc']);    					
    				}
    			}
    		}
    		foreach($rs as $f){
    			$fomid = $f['fomid'];
    			$grupos[$f['pergrupo']] = $f['pergrupo'];
    			$dados[$f['pergrupo']][$f['perid']]['grupo'] = $f['pergrupo'];
    			$dados[$f['pergrupo']][$f['perid']]['id'] = $f['perid'];
    			$dados[$f['pergrupo']][$f['perid']]['nome'] = $f['pernome'];
    			$dados[$f['pergrupo']][$f['perid']]['ajuda'] = $f['perajuda'];
    			$dados[$f['pergrupo']][$f['perid']]['tipo'] = $f['pertipo'];
    			$dados[$f['pergrupo']][$f['perid']]['obrigatorio'] = $f['perobrigatorio'];
    			$dados[$f['pergrupo']][$f['perid']]['itmid'] = $arResposta[$f['perid']]['itmid'];
    			$dados[$f['pergrupo']][$f['perid']]['reidsc'] = $arResposta[$f['perid']]['reidsc'];
    			$dados[$f['pergrupo']][$f['perid']]['class'] = $f['perobrigatorio'] ? $f['perclasse'].' required' : $f['perclasse'];																;
    		}
    		if($dados){
    			$x=0;
    			$html = '';
    			foreach($grupos as $grupo){
    				$x++;
    				$html .= '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">';
    				foreach($dados[$grupo] as $item){
    					if($item['tipo']=='titulo'){
    						$html .= '<tr><td colspan="2" class="subtitulocentro"><span class="'.$item['class'].'"><font size="+1">'.$item['nome'].'</font></span></td></tr>';
    					}else
    					if($item['tipo']=='subtitulo'){
    						$html .= '<tr><td colspan="2" class="subtitulocentro"><span class="'.$item['class'].'">'.$item['nome'].'</span></td></tr>';
    					}else
    					if($item['tipo']=='grid'){
    						$html .= '<tr><td colspan="2" align="center">'.$item['reidsc'].'</td></tr>';
    					}else{
    						$html .= '
    								<tr>
										<td width="50%" class="subtitulodireita">
    									'.$item['nome'].'<br/>
    									<font class="txtajuda">'.$item['ajuda'].'</font>
    									</td>
										<td>'.$item['reidsc'].'</td>
									</tr>
    								';
    					}
    				}
    			}
    				$html .= '<tr><td colspan="2" algin="center">
    						<center>
							<input type="button" value="Voltar" class="btnVoltar" onclick="javascript:history.back(-1)" />
							</center>
							</td></tr>
    						</table><p>&nbsp;</p>';
    		}
    	}
    	echo $html;
    }
    
    function salvaFormulario($dados)
    {
    	//ver($dados, d);
    	$idGrupo = $dados['pergrupo'];
    	$rs = $this->recuperaPerguntas(null, $idGrupo);
    	$fomid = $dados['fomid'];
    	if($rs){
    		
    		foreach($rs as $v){
    			$arPerid[$v['perid']] = $v['perid'];
    		}
    		
    		$envia = $dados['envia']=='true' ? 'now()' : 'null';
    		
    		if($dados['resid']){
    			
    			$resid = $dados['resid'];
    			
    			$sqlRes = "update maismedicos.form_resposta set
    						resdtatualiza = now(),
    						usucpf = '{$_SESSION['usucpf']}',
    						resdtenvio = {$envia}
    					where resid = {$resid};";
    			
    			$this->pegaUm($sqlRes);
    			
    		}else{
    			
    			$sqlRes = "insert into maismedicos.form_resposta    					
		    			(fomid,resdtatualiza,usucpf, resdtenvio) values
		    			({$fomid}, now(), '{$_SESSION['usucpf']}', {$envia}) returning resid;";
    			
    			$resid = $this->pegaUm($sqlRes);
    		}
    		//echo $resid;
    		//die;
    		
    		$sql .= "
    				delete from maismedicos.form_resposta_item 
		    		where perid in (".implode(',', $arPerid).")
		    		and resid = {$resid};
		    		";
    		//ver($dados, d);
    		foreach($rs as $p){
    			if(!in_array($p['pertipo'], array('titulo','subtitulo'))){
	    			if(is_array($dados[$p['perid']])){
	    				foreach($dados[$p['perid']] as $k => $value){
	    					$item = '';
	    					$desc = '';
	    					if(trim($p['pertipo'])=='grid'){
	    						$item = $k;
	    						$desc = trim($value)>0 ? $value : '';
	    					}else{
	    						$item = $value;
	    						$desc = $dados['outro'][$p['perid']][$item] ? utf8_encode($dados['outro'][$p['perid']][$item]) : '';
	    					}
	    				
	    					if(!empty($item) || !empty($desc)){
			    				$sql .= "insert into maismedicos.form_resposta_item
			    						(perid,itmid,reidsc,resid) values ({$p['perid']},
			    						 ".($item ? $item : 'null').", '{$desc}', {$resid});";
	    					}
	    					
	    				}
	    			}else{
	    				
	    				$value = in_array($p['pertipo'], array('integer', 'cpf')) ? str_replace(array('.','-'), '', trim($dados[$p['perid']])) : trim($dados[$p['perid']]);
	    				$value = $p['pertipo'] == 'date' ? formata_data_sql(trim($dados[$p['perid']])) : $value;
	    				
	    				if(in_array($p['pertipo'], array('select', 'radio'))){
	    					$item = $value;
	    					$desc = '';
	    				}else{
	    					$item = '';
	    					$desc = $value;
	    				}
	    				
	    				if(!empty($item) || !empty($desc)){
			    			$sql .= "insert into maismedicos.form_resposta_item
			    					(perid,itmid,reidsc,resid) values ({$p['perid']}, ".($item ? $item : 'null').", '{$desc}', {$resid});";
	    				}
	    			}
    			}
    			
    		}
    		
    		if($sql){
    			$this->executar($sql);
    			$this->commit();
    		}
    	}
    }
    
    function montaCampo($dados = array())
    {
    	switch($dados['tipo']){
    		case 'municipio':
    			return $this->campoMunicipio($dados);
    			break;
    		case 'date':
    			return $this->campoDate($dados);
    			break;
    		case 'cpf':
    			return $this->campoCpf($dados);
    			break;
    		case 'integer':
    			return $this->campoInteger($dados);
    			break;
    		case 'mesreferencia':
    			return $this->campoMesReferencia($dados);
    			break;
    		case 'ano':
    			return $this->campoAno($dados);
    			break;
    		case 'autocomplete':
    			return $this->campoAutocomplete($dados);
    			break;
    		case 'text':
    			return $this->campoText($dados);
    			break;
    		case 'textarea':
    			return $this->campoTextarea($dados);
    			break;
    		case 'grid':
    			return $this->campoGrid($dados);
    			break;
    		case 'select':
    			return $this->campoSelect($dados);
    			break;
    		case 'checkbox':
    			return $this->campoCheckbox($dados);
    			break;
    		case 'radio':
    			return $this->campoRadio($dados);
    			break;
    		case 'uf':
    			return $this->campoUF($dados);
    			break;
    		default:
    			return $dados['tipo'];
    			break;
    	}
    }
    
    function campoAutocomplete($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['size'] = $dados['size'] ? $dados['size'] : 25;
    		$html .= '<input type="text" name="nome_'.$dados['id'].'" id="nome_'.$dados['id'].'" class="'.$dados['class'].'" size="'.$dados['size'].'" value="'.$dados['reidsc'].'" />';
    		$html .= '<input type="hidden" name="'.$dados['id'].'" id="'.$dados['id'].'" value="'.$dados['reidsc'].'" />';
    	}
    	return $html;
    }
    
    function campoDate($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['size'] = $dados['size'] ? $dados['size'] : 25;
    		$html .= campo_data2( $dados['id'], 'N', 'S', '', 'N','','', formata_data($dados['reidsc']) );
    		$html .= '<span class="'.$dados['class'].'">&nbsp;</span>';
    	}
    	return $html;
    }
    
    function campoInteger($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['size'] = $dados['size'] ? $dados['size'] : 25;
    		$html .= campo_texto($dados['id'], '', 'S', '', '12', '16', '[.###]', '', '', '', '', '', '', $dados['reidsc']);
    	}
    	return $html;
    }
    
    function campoCpf($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['size'] = $dados['size'] ? $dados['size'] : 25;
    		//$html .= campo_texto($dados['id'], '', 'S', '', '25', '', '', '', '', '', '', '', '', trim($dados['reidsc']));
    		$html .= '<input type="text" name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" value="'.$dados['reidsc'].'" size="25" 
    				onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal(\'###.###.###-##\',this.value);" />';
    		
//<input type="text" style="text-align:left;" name="lcbcpftutorsolicitante" size="21" maxlength="14" value="" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('###.###.###-##',this.value);" title="" class="obrigatorio normal required">
    	}
    	return $html;
    }
    
    function campoText($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['size'] = $dados['size'] ? $dados['size'] : 25;
    		//$html .= campo_texto($dados['id'], '', 'S', '', '25', '', '', '', '', '', '', '', '', trim($dados['reidsc']));
    		$html .= '<input type="text" name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" value="'.$dados['reidsc'].'" size="25" />';
    	}
    	return $html;
    }
    
    function campoTextarea($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$dados['cols'] = $dados['cols'] ? $dados['cols'] : 45;
    		$dados['rows'] = $dados['rows'] ? $dados['rows'] : 8;
    		$html .= '<textarea name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" cols="'.$dados['cols'].'" rows="'.$dados['rows'].'">'.trim($dados['reidsc']).'</textarea>';
    	}
    	return $html;
    }
    
    function campoSelect($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		
    		$sql = "select * from maismedicos.form_pergunta_item where itmstatus = 'A' and perid = {$dados['id']}";
    		$rsItens = $this->carregar($sql);
    		$html .= '<select name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" />';
    		if($rsItens){
    			$html .= '<option value="">Selecione...</option>';
    			foreach($rsItens as $item){
    				$html .= '<option value="'.$item['itmid'].'" '.(trim($dados['itmid']) == $item['itmid'] ? 'selected' : '').'>'.$item['itmdsc'].'</option>';
    			}
    		}else{
    			$html .= '<option value=""></option>';
    		}
    		$html .= '</select>';
    	}
    	return $html;
    }
    
    function campoMunicipio($dados = null)
    {
    	//ver($dados);
    	$html = '';
    	if($dados['id']){
    		
    		if($dados['reidsc']){
    		
	    		$sql = "select muncod as codigo, mundescricao as descricao from territorios.municipio 
	    				where estuf = (select estuf from territorios.municipio where muncod = '{$dados['reidsc']}')";
	    		$rsItens = $this->carregar($sql);
    		}
    		
    		$html .= '<select name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" />';
    		if($rsItens){
    			$html .= '<option value="">Selecione...</option>';
    			foreach($rsItens as $item){
    				$html .= '<option value="'.$item['codigo'].'" '.(trim($dados['reidsc']) == $item['codigo'] ? 'selected' : '').'>'.$item['descricao'].'</option>';
    			}
    		}else{
    			$html .= '<option value=""></option>';
    		}
    		$html .= '</select>';
    	}
    	return $html;
    }
    
    function campoGrid($dados = null)
    {
    	$dados['itmid'] = $dados['itmid'] ? $dados['itmid'] : array();
    	$dados['reidsc'] = $dados['reidsc'] ? $dados['reidsc'] : array();
    	$html = '';
    	if($dados['id']){
    		$sql = "select * from maismedicos.form_pergunta_item where itmstatus = 'A' and perid = {$dados['id']}";
    		$rsItens = $this->carregar($sql);
    		
    		$sql = "select * from maismedicos.form_pergunta_subitem where suistatus = 'A' and perid = {$dados['id']}";
    		$rsSubItens = $this->carregar($sql);
    		if($rsItens){
    			$html .= '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%">';
    			$html .= '<tr><td></td>';
    			foreach($rsSubItens as $sub){
    				$html .= '<td><center>'.$sub['suidsc'].'</center></td>';
    			}
    			
    			$html .= '</tr>';
    			foreach($rsItens as $k => $item){
    				$html .= '<tr><td>'.$item['itmdsc'].'</td>';
    				foreach($rsSubItens as $sub){
	    				$checked = '';
    					$html .= '<td><center><input type="radio" name="'.$item['perid'].'['.$item['itmid'].']" 
    								id="'.$item['perid'].'_'.$sub['itmid'].'" value="'.$sub['suiid'].'"';
    					
    					foreach($dados['reidsc'] as $k => $v){
    						if($k.$v == $item['itmid'].$sub['suiid']){
    							$checked = 'checked';
    						}
    					}
    					$html .= $checked.'/></center></td>';
    				}
    				$html .= '</tr>';
    			}
    			$html .= '</table>';
    		}
    	}
    	return $html;
    }
    
    function campoCheckbox($dados = null)
    {
    	//ver($dados);
    	$html = '';
    	if($dados['id']){
    		$sql = "select * from maismedicos.form_pergunta_item where itmstatus = 'A' and perid = {$dados['id']}";
    		$rsItens = $this->carregar($sql);
    		if($rsItens){
    			$dados['itmid'] = $dados['itmid'] ? $dados['itmid'] : array();
    			foreach($rsItens as $k => $item){
    				$html .= '<input type="checkbox" name="'.$dados['id'].'[]" id="'.$dados['id'].'_'.$k.'" 
    						value="'.$item['itmid'].'" class="'.$dados['class'].'" 
    								'.(in_array($item['itmid'], $dados['itmid']) ? 'checked' : '').'/>&nbsp;'.$item['itmdsc'].'';
    				if(strtolower(trim($item['itmdsc']))=='outro'){
    					$html .= '&nbsp;<input type="text" name="outro['.$dados['id'].']['.$item['itmid'].']" 
    							id="outro_'.$dados['id'].'" size="10"  value="'.$dados['reidsc'][$item['itmid']].'" />';
    				}
    				$html .= '</br>';
    			}
    		}
    	}
    	return $html;
    }
    
    function campoRadio($dados = null)
    {
    	$html = '';
    	if($dados['id']){
    		$sql = "select * from maismedicos.form_pergunta_item where itmstatus = 'A' and perid = {$dados['id']}";
    		$rsItens = $this->carregar($sql);
    		if($rsItens){
    			foreach($rsItens as $k => $item){
    				$html .= '<input type="radio" name="'.$dados['id'].'[]" id="'.$dados['id'].'_'.$k.'" 
    							value="'.$item['itmid'].'" class="'.$dados['class'].'" />&nbsp;'.$item['itmdsc'].'';
    				if(strtolower(trim($item['itmdsc']))=='outro'){
    					$html .= '&nbsp;<input type="text" name="outro['.$dados['id'].']['.$item['itmid'].']" id="outro_'.$dados['id'].'" size="10" />';
    				}
    				$html .= '</br>';
    			}
    		}
    	}
    	return $html;
    }
    
    function campoMesReferencia($dados = null, $anoIni = 2014, $anoFim = null )
    {
    	$anoFim = $anoFim ? $anoFim : date('Y')+2;
    	
    	$arData = explode('/', $dados['reidsc']);
    	$mes = $arData[0];
    	$ano = $arData[1];
    	
    	$html = '';
    	if($dados['id']){
    		$html .= '<select name="mes_'.$dados['id'].'", id="mes_'.$dados['id'].'" class="'.$dados['class'].'">';
    		$html .= '<option value="">Selecione...</option>';
    		for($m=1;$m<13;$m++){
    			$html .= '<option value="'.$m.'" '.($m==$mes ? 'selected' : '').'>'.mes_extenso($m).'</option>';
    		}
    		$html .= '</select><select name="ano_'.$dados['id'].'", id="ano_'.$dados['id'].'" class="'.$dados['class'].'">';
    		$html .= '<option value="">Selecione...</option>';
    		for($x=$anoIni;$x<$anoFim;$x++){
    			$html .= '<option value="'.$x.'" '.($x==$ano ? 'selected' : '').'>'.$x.'</option>';
    		}
    		$html .= '</select>';
    		$html .= '<input type="hidden" class="mesref" name="'.$dados['id'].'" id="'.$dados['id'].'" value="'.($dados['reidsc'] ? $dados['reidsc'] : '').'" />';
    	}
    	return $html;
    }
    
    function campoAno($dados = null, $anoIni = 1970, $anoFim = null)
    {
    	$anoFim = $anoFim ? $anoFim : date('Y');
    	
    	$html = '';
    	if($dados['id']){
    		$html .= '<select name="'.$dados['id'].'" id="ano_'.$dados['id'].'" >';
    		$html .= '<option value="">Selecione...</option>';
    		for($x=$anoIni;$x<$anoFim;$x++){
    			$html .= '<option value="'.$x.'" '.($x==trim($dados['reidsc']) ? 'selected' : '').'>'.$x.'</option>';
    		}
    		$html .= '</select>';
    	}
    	return $html;
    }
    
    function campoUF($dados = null)
    {
    	
    	$html = '';
    	if($dados['id']){
    		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf";
    		$rsItens = $this->carregar($sql);
    		$html .= '<select name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.$dados['class'].'" />';
    		if($rsItens){
    			$html .= '<option value="">Selecione...</option>';
    			foreach($rsItens as $item){
    				//ver($item['codigo'], trim($dados['reidsc']));
    				$html .= '<option value="'.$item['codigo'].'" '.(trim($item['codigo'])==trim($dados['reidsc']) ? 'selected' : '').'>'.$item['descricao'].'</option>';
    			}
    		}else{
    			$html .= '<option value=""></option>';
    		}
    		$html .= '</select>';
    	}
    	return $html;
    }
    

    function carregaTutores($dados){
    
    	$sql = "select
				tutcpf as codigo,
				tutcpf || ' - ' || tutnome as descricao
			from maismedicos.tutor
			where tutstatus = 'A'
			and tuttipo = 'T'
			".($dados["q"] ? " AND tutnome ILIKE '%{$dados["q"]}%' or tutcpf ILIKE '%{$dados["q"]}%'" : "")."
			order by tutnome ";
    
    	$arDados = $this->carregar($sql);
    
    	$q = strtolower($dados["q"]);
    
    	if($arDados){
    		foreach ($arDados as $key=>$value){
    			echo "{$value['descricao']}|{$value['codigo']}\n";
    		}
    	}else{
    		echo "Sem registros|";
    	}
    	die;
    }
    
    
    function carregaSupervisores($dados){
    
    	$sql = "select
				tutcpf as codigo,
				tutcpf || ' - ' || tutnome as descricao
			from maismedicos.tutor
			where tutstatus = 'A'
			and tuttipo = 'S'
			".($dados["q"] ? " AND tutnome ILIKE '%{$dados["q"]}%' or tutcpf ILIKE '%{$dados["q"]}%'" : "")."
			order by tutnome ";
    
    	$arDados = $this->carregar($sql);
    
    	$q = strtolower($dados["q"]);

    	if($arDados){
    		foreach ($arDados as $key=>$value){
    			echo "{$value['descricao']}|{$value['codigo']}\n";
    		}
    	}else{
    		echo "Sem registros|";
    	}
    	die;
    }
    
    function carregaMedicos($dados){
    
    	$sql = "select
				mdccpf as codigo,
				mdccpf || ' - ' || mdcnome as descricao
			from maismedicos.medico
			where mdcstatus = 'A'
			".($dados["q"] ? " AND mdcnome ILIKE '%{$dados["q"]}%' or mdccpf ILIKE '%{$dados["q"]}%'" : "")."
			order by mdcnome ";
    
    	$arDados = $this->carregar($sql);
    
    	$q = strtolower($dados["q"]);

    	if($arDados){
    		foreach ($arDados as $key=>$value){
    			echo "{$value['descricao']}|{$value['codigo']}\n";
    		}
    	}else{
    		echo "Sem registros|";
    	}
    	die;
    }
    

    function carregaMunicipios($dados){
    
    	$sql = "SELECT muncod as codigo, mundescricao as descricao
    	FROM territorios.municipio
    	WHERE estuf = '{$dados['uf']}'
    	ORDER BY 2 ASC";
    	$rs = $this->carregar($sql);
    	//$this->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
    	
    	$html .= '<select name="'.$dados['id'].'" id="'.$dados['id'].'" class="'.str_replace('.','',$dados['class']).'">';
    	$html .= '<option value="">Selecione...</option>';
    	foreach($rs as $m){
    		$html .= '<option value="'.$m['codigo'].'">'.$m['descricao'].'</option>';
    	}
    	$html .= '</select>';
    	echo $html;
    }
    
    function carregaInstituicoes($dados){
    
    	$sql = "select
				uniid as codigo,
				uninome as descricao
			from maismedicos.universidade
			".($dados["q"] ? "WHERE uninome ILIKE '%{$dados["q"]}%'" : "")."";
    
    	$arDados = $this->carregar($sql);
    
    	$q = strtolower($dados["q"]);
    
    	if($arDados){
    		foreach ($arDados as $key=>$value){
    			echo "{$value['descricao']}|{$value['codigo']}\n";
    		}
    	}else{
    		echo "Sem registros|";
    	}
    	die;
    }
    
    function carregaDadosMedico($dados){
    
    	$sql = "select
			    	mdccpf,
			    	mdcnome,
			    	mdcemail,
			    	mdcnomeinstituicao
		    	from maismedicos.medico
		    	where mdcstatus = 'A'
		    	and mdccpf = '{$dados['cpf']}'
		    	order by mdcnome ";
    
    	$rs = $this->pegaLinha($sql);
    	$arDados['mdccpf'] = $rs['mdccpf'];
    	$arDados['mdcnome'] = utf8_encode($rs['mdcnome']);
    	$arDados['mdcemail'] = $rs['mdcemail'];
    	$arDados['mdcnomeinstituicao'] = utf8_encode($rs['mdcnomeinstituicao']);
		echo json_encode($arDados);
    	die;
    }
    

    function carregaDadosSupervisor($dados){
    
    	$sql = "select
			    	tutcpf,
			    	tutnome
		    	from maismedicos.tutor
		    	where tutstatus = 'A'
		    	and tutcpf = '{$dados['cpf']}'
		    	order by tutnome ";
    	
    	$sql = "select 
    			nosupervisor as nome,
				nucpf as cpf, 
				deemail as email,
				dstitulacao as titulacao,
				idinstituicao,
				noinstituicao,
				uniid,
				uninome
    			from maismedicos.ws_supervisores s
    			left join maismedicos.universidade u on u.idunasus = s.idinstituicao
				where wssstatus = 'A'
    			and nucpf = '{$dados['cpf']}'
				and to_char(wssdata, 'YYYYMMDD') = (select to_char(max(wssdata),'YYYYMMDD') from maismedicos.ws_supervisores where wssstatus = 'A')";
    
    	$rs = $this->pegaLinha($sql);
    	$arDados['cpf'] = $rs['cpf'];
    	$arDados['nome'] = utf8_encode($rs['nome']);
    	$arDados['email'] = $rs['email'];
    	$arDados['titulacao'] = utf8_encode($rs['titulacao']);
    	$arDados['uniid'] = $rs['uniid'];
    	$arDados['uninome'] = utf8_encode($rs['uninome']);
    	echo json_encode($arDados);
    	die;
    }
    
    public function recuperaTutorPorPlanoTrabalho($cpf = null)
    {
    	if($cpf){
    		
	    	$sql = "select * from maismedicos.ws_planotrabalho_itens where to_char(wssdata,'YYYYMMDD') = 
					(select to_char(max(wssdata),'YYYYMMDD') from maismedicos.ws_planotrabalho_itens where wssstatus = 'A')
	    			and cpfsupervisor = '{$cpf}'
	    			order by wssdata desc";
	    	
	    	return $this->pegaUm($sql);
    	}
    	return false;
    }
    
    public function recuperaUltimoRelatorioAberto()
    {
    	$sql = "select resid from maismedicos.form_resposta where resstatus = 'A' and usucpf = '{$_SESSION['usucpf']}'";
    	return $this->pegaUm($sql);	
    }
    
    
}