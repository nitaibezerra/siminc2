<?php
	
class Planotrabalho extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.planotrabalho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptrid' => null, 
									  	'ptridpai' => null, 
									  	'arqid' => null, 
									  	'benid' => null, 
									  	'perid' => null, 
									  	'ptrcpf' => null, 
									  	'ptrnome' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'ptrmes' => null, 
									  	'ptrsemestre' => null, 
									  	'ptrano' => null, 
									  	'ptrdtcadastro' => null, 
									  	'ptrdtalterado' => null, 
									  	'ptrdtenvio' => null, 
									  	'ptrobs' => null, 
									  	'ptrstatus' => null, 
									  );
    
    public function cabecalhoPlanoTrabalho()
    {
    }
    
    public function verificaPlanoTrabalhoAberto($post = null)
    {    	
    	if($post['ptrid']){
    		$arWhere[] = "ptrid = {$post['ptrid']}";
    	}else
    	if($post['perid']){
    		$arWhere[] = "perid = {$post['perid']}";
    	}
    	$sql = "select * from {$this->stNomeTabela} where ptrstatus = 'A' and ptrdtenvio is null
    			and ptrcpf = '{$_SESSION['usucpf']}' 
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." order by ptrid desc";
    	
    	return $this->pegaLinha($sql);
    }
    
    public function verificaPlanoTrabalhoMesAno($mes = null, $ano = null, $semestre = null)
    {
    	if($mes)
    		$arWhere[] = "ptrmes = '{$mes}'";
    	if($ano)
    		$arWhere[] = "ptrano = '{$ano}'";
    	if($semestre)
    		$arWhere[] = "ptrsemestre = '{$semestre}'";
    	
    	$sql = "select true from {$this->stNomeTabela} where ptrstatus = 'A' 
    	and ptrcpf = '{$_SESSION['usucpf']}' ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
    	return $this->pegaUm($sql);
    }
    
    public function contaPlanosPorPeriodo($dados = null)
    {
    	if($dados['ptrmes'])
    		$arWhere[] = "ptrmes = '{$dados['ptrmes']}'";
    	if($dados['ptrsemestre'])
    		$arWhere[] = "ptrsemestre = '{$dados['ptrsemestre']}'";
    	if($dados['ptrano'])
    		$arWhere[] = "ptrano = '{$dados['ptrano']}'";
    	 
    	$sql = "select count(ptrid) from {$this->stNomeTabela} where ptrstatus = 'A' and ptrdtenvio is not null
    	and ptrcpf = '{$_SESSION['usucpf']}' ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
    			return $this->pegaUm($sql);
    }
    
    public function verificaPlanoSemestre($dados = null)
    {
    	if($dados['ptrid'] && $dados['ptrsemestre']){
    		$arWhere[] = "ptrid not in ({$dados['ptrid']})";
    		$arWhere[] = "ptrsemestre = '{$dados['ptrsemestre']}'";
	    	$sql = "select count(*) from {$this->stNomeTabela}
	    			 where ptrstatus = 'A' 
	    			 and ptrdtenvio is not null
	    			 and ptrcpf = '{$_SESSION['usucpf']}'".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
	    	return $this->pegaUm($sql);
    	}
    	return false;
    }
    
    public function recuperaPlanosPorCpf($cpf = null, $enviado =false)
    {
    	$sql = "select 
    				 
    				'
    				<img title=\"Visualizar\" src=\"../imagens/lupa_grafico.gif\" style=\"cursor:pointer;\" class=\"btnDetalhe\" id=\"' || ptr.ptrid || '\" />
    				'  as acao,
					case when ptrmes is not null then '' ||  ptrmes || '/' || ptrano || '' end as mes,
					case when perdtfim is null then to_char(perdtini,'DD/MM/YYYY') else to_char(perdtini,'DD/MM/YYYY') || ' até ' || to_char(perdtfim,'DD/MM/YYYY') end as periodo,
					'<a href=\"maismedicos.php?modulo=principal/pagamento/listPlanoTrabalho&acao=A&&requisicao=montaDetalhePlanoTrabalho&semestre=' || ptr.ptrid || '_' || ptrano || '_' || ptrsemestre || '_' || ptrcpf || '\" class=\"btnSemestre\" >' || case when ptrsemestre = 1 then '1º Semestre' when ptrsemestre = 2 then '2º Semestre' else ptrsemestre::varchar end || ' de ' || ptrano || '</a>' as semestre,
					case when ptrdtenvio is null then 'Não enviado' else to_char(ptrdtenvio, 'DD/MM/YYYY') end as dtenvio,
					(select count(*) from maismedicos.planotrabalho_visitaprogramada ptv where ptv.ptrid = ptr.ptrid and ptv.ptvstatus = 'A') as visitas, 
					(select count(*) from maismedicos.planotrabalho_atividadeacompanhamento pta where pta.ptrid = ptr.ptrid and pta.ptastatus = 'A') as atividades,
					'<center><img src=\"../imagens/exclui_p.gif\" /></center>' as pago 
    	 		from {$this->stNomeTabela} ptr
    	 		left join maismedicos.pagamento_periodo per on per.perid = ptr.ptrid
		    	where ptrstatus = 'A' 
		    	and ptrcpf = '{$_SESSION['usucpf']}'
		    	".($enviado ? 'and ptrdtenvio is not null' : 'and ptrdtenvio is null')."
		    	order by ptrano desc, ptrsemestre desc, ptrmes desc";
    	
    	return $this->carregar($sql);
    }
    
    public function abrePlano($post = null, $id = null)
    {
    	
    	$benid = $this->cadastraBeneficiario();
    	
    	$sql = "select benid, muncod, estuf from maismedicos.pagamento_beneficiario where benid = {$benid};";
    	$ben = $this->pegaLinha($sql);
    	
    	if($id) $post['ptrid'] = $id;
    	$plano = $this->verificaPlanoTrabalhoAberto($post);

    	if(!$plano){    	
	    	$arDados['perid'] 	= $post['perid'];
	    	$arDados['benid'] 	= $ben['benid'];
	    	$arDados['ptrmes'] 	= $post['permes'];
	    	$arDados['ptrsemestre'] = $post['permes'] < 7 ? 1 : 2;
	    	$arDados['ptrano'] 	= $post['perano'];
	    	$arDados['ptrcpf'] 	= $_SESSION['usucpf'];
	    	$arDados['ptrnome'] = $_SESSION['usunome'];
	    	$arDados['muncod'] 	= $ben['muncod'];
	    	$arDados['estuf'] 	= $ben['estuf'];
	    	$this->popularDadosObjeto($arDados);
	    	$id = $this->salvar();
	    	$this->commit();
	    	$plano = $this->verificaPlanoTrabalhoAberto(array('ptrid'=>$id));
    	}
    	    	
    	return $plano;
    }
    
    public function verificaExisteBeneficiario($cpf = null)
    {
    	$sql = "select benid from maismedicos.pagamento_beneficiario where bencpf = '{$cpf}'";
    	return $this->pegaUm($sql);
    }
    
    public function cadastraBeneficiario()
    {
    	$benid = $this->verificaExisteBeneficiario($_SESSION['usucpf']);
    	if(!$benid){
    	
    		$sql = "select usucpf,usunome,usuemail,usufoneddd,usufonenum,usufuncao,ususexo,orgcod,
		    		unicod,ungcod,pflcod,orgao,m.muncod,m.estuf,usudatanascimento,carid
		    		from seguranca.usuario u
		    		left join territorios.municipio m on m.muncod = u.muncod
		    		where usucpf = '{$_SESSION['usucpf']}'";
    		
    		$usu = $this->pegaLinha($sql);
    		
    		if($usu){
    			include_once APPRAIZ . 'maismedicos/classes/Pagamento_Beneficiario.class.inc';
    			$obBen = new Pagamento_Beneficiario();
    			
    			$perfis = arrayPerfil();
    			if(in_array(PERFIL_COORDENADOR_CAMEM, $perfis)){
    				$funcao = FUNCAO_COORDENADOR_CAMEM;
    			}elseif(in_array(PERFIL_AVALIADOR_CAMEM, $perfis)){
    				$funcao = FUNCAO_AVALIADOR_CAMEM;
    			}else{
    				$funcao = null;
    			}
    			
    			$arDados['uniid'] = null;
    			$arDados['funid'] = $funcao;
    			$arDados['bencpf'] = $usu['usucpf'];
    			$arDados['bennome'] = $usu['usunome'];
    			$arDados['bendatanascimento'] = $usu['usudatanascimento'];
    			$arDados['benemail'] = $usu['usuemail'];
    			$arDados['bentelefone'] = $usu['usufoneddd'].''.$usu['usufonenum'];
    			$arDados['bennomemae'] = null;
    			$arDados['bencep'] = null;
    			$arDados['benlogradouro'] = null;
    			$arDados['bennumero'] = null;
    			$arDados['bencomplemento'] = null;
    			$arDados['benbairro'] = null;
    			$arDados['estuf'] = $usu['estuf'];
    			$arDados['muncod'] = $usu['muncod'];
    			$arDados['bncid'] = null;
    			$arDados['benagencia'] = null;
    			$arDados['benconta'] = null;
    			$arDados['benstatus'] = null;
    			$arDados['benvalidade'] = null;
    			$arDados['bendataatualizacao'] = null;
    			$arDados['bennumbeneficio'] = null;
    			
    			$obBen->popularDadosObjeto($arDados);
    			$benid = $obBen->salvar();
    			$obBen->commit();
    		}
    	}
    	//ver($benid);
    	return $benid;
    }
    
    public function salvaPlanoCoordAvaliador($post = null, $files = null)
    {
    	$arDados['ptrid'] 	= $post['ptrid'];
    	$arDados['arqid'] 	= $post['arqid'];
    	$arDados['ptrobs'] 	= $post['ptrobs'];
    	$arDados['ptrdtalterado'] = date('Y-m-d H:i:s');
    	$arDados['usucpf'] 	= $_SESSION['usucpf'];
    	
    	if($post['enviar_plano']=='true'){
    		$arDados['ptrdtenvio'] = date('Y-m-d H:i:s');    		
    	}
    	
    	$this->popularDadosObjeto($arDados);
    	$ptrid = $this->salvar();
    	$this->commit();
    	
    	include_once APPRAIZ.'maismedicos/classes/Planotrabalho_VisitaProgramada.class.inc';
    	$obVp = new Planotrabalho_VisitaProgramada();
    	$obVp->salvaVisitaProgramadaPT($post, $files);
    	
    	include_once APPRAIZ.'maismedicos/classes/Planotrabalho_AtividadeAcompanhamento.class.inc';
    	$obAt = new Planotrabalho_AtividadeAcompanhamento();
    	$obAt->salvaAtividadeAcompanhamentoPT($ptrid, $post, $files);
    }
    
    public function excluiPlano($ptrid = null)
    {
    	if($ptrid){
    		$sql = "update {$this->stNomeTabela} set ptrstatus = 'I' where ptrid = {$ptrid};";
    		$this->executar($sql);
    		$this->commit();
    		return true;
    	}
    	return false;
    }
    
    public function recuperaPlanoTrabalho($post = null)
    {
    	if($post['bencpf']){
    		$arWhere[] = "p.ptrcpf = '{$post['bencpf']}'";
    		if($post['bensemestre']){
	    		$arWhere[] = "p.ptrsemestre = '{$post['bensemestre']}'";
    		}
    	}else    	
    	if($post['ptrid']){
    		$arWhere[] = "p.ptrid = '{$post['ptrid']}'";
    	}else{
    		if($post['mes'])
    			$arWhere[] = "ptrmes = '{$post['mes']}'";
    		if($post['ano'])
    			$arWhere[] = "ptrano = '{$post['ano']}'";
    		if($post['cpf'])
    			$arWhere[] = "ptrcpf = '{$post['cpf']}'";
    	}
    	 
    	$sql = "select p.*, e.ptedtenvio, e.pteobs, e.pteid, e.ptemes, e.pteano, e.ptesemestre from maismedicos.planotrabalho p
    			left join maismedicos.planotrabalho_execucao e on e.ptrid = p.ptrid
    				and ptedtenvio is not null
    			where ptrstatus = 'A'    			
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
    			order by ptrano desc, ptrmes desc";
    	//ver($sql);
    	return $this->pegaLinha($sql);
    }
    
    public function montaDetalhePlanoTrabalho($post = null)
    {

    	if($post['semestre']){
    		$arSem = explode('_',$post['semestre']);
    		$ptridItens = $arSem[0];
    		$ano = $arSem[1];
    		$semestre = $arSem[2];
    		$cpf = $arSem[3];
    		$post['ano'] = $ano;
    		$post['cpf'] = $cpf;
    		$post['semestre'] = $semestre;
    		$post['ptrid'] = $ptridItens;
    	}
    	
    	$plano = $this->recuperaPlanoTrabalho($post);
    	$post['ptrid_itens'] = $plano['ptrid'];
    	
    	if($plano){
    		$html .= '
				<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
				    <tr>
				        <td class="subtitulodireita" width="150" valign="top">Nome</td>
				        <td valign="top">'.formatar_cpf($plano['ptrcpf']).' - '.$plano['ptrnome'].'</td>
				    </tr>
				    <tr>
				        <td class="subtitulodireita" valign="top">Período</td>
				        <td valign="top">'.$plano['ptrsemestre'].'º semestre de '.$plano['ptrano'].'</td>
				    </tr>';
    		if($post['ptrid']){
    			$html .= '<tr>
					        <td class="subtitulodireita" valign="top">Mês de referência</td>
					        <td valign="top">'.$plano['ptrmes'].'/'.$plano['ptrano'].'</td>
					    </tr>';
    		}
		    $html .= '</table>';
				
			$html .= $this->montaDetalheVisita($post,true,true,false);	
			$html .= $this->montaDetalheVisita($post,false,false,true);	
			$html .= $this->montaDetalheAtividade($post,true,true,false);	
			$html .= $this->montaDetalheAtividade($post,false,false,true);	
			$html .= $this->montaDetalheObservacoes($post);
    	
    	}
    	return $html;
    }
    
    public function montaDetalheObservacoes($post = null)
    {
    	if($post['ptrid']){
    		$arWhere[] = "p.ptrid = {$post['ptrid']}";
    	}else
    	if($post['semestre']){
    		$arWhere[] = "p.ptrano = {$post['ano']} and ptrsemestre = '{$post['semestre']}' and ptrcpf = '{$post['cpf']}'";
    	}
    	
    	$sql = "select ptrobs, pteobs, ptrmes, ptrano,ptrdtenvio, ptedtenvio from maismedicos.planotrabalho p 
    			left join maismedicos.planotrabalho_execucao e on p.ptrid = e.ptrid
    				and e.ptestatus = 'A'
    			where p.ptrstatus = 'A'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
    			order by ptrano desc, ptrmes desc";
    	
    	$rsPtr = $this->carregar($sql);
    	
    	if($rsPtr){
    		$html = '<table align="center" class="tabela" id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;">';
    		$html .= '<tr><td class="subtitulocentro">Observações</td></tr>';
    		foreach($rsPtr as $ptr){
    			$html .= '<tr><td valign="top">
    						'.($post['ptrid'] ? '' : '<p><b>Mês de ref:</b> '.$ptr['ptrmes'].'/'.$ptr['ptrano'].'</p>').'
    						<p><b>Plano/Ajuste ('.($ptr['ptrdtenvio']?'<font color="green">Enviado':'<font color="red">Não enviado').'</font>)</b>: '.($ptr['ptrobs'] ? $ptr['ptrobs'] : 'Sem observações').'</p>
    						<p><b>Execução ('.($ptr['ptedtenvio']?'<font color="green">Enviado':'<font color="red">Não enviado').'</font>):</b> '.($ptr['pteobs'] ? $ptr['pteobs'] : 'Sem observações').'</p>
    						</td></tr><tr style="height:25px">&nbsp;<td></td></tr>';
    		}
    		$html .= '</table>';
    	}
		return $html;    	
    }
    
    public function montaDetalheVisita($post = null, $ativo = true, $ini = true, $fim = true)
    {
    	if($post['bencpf']){
    		$arWhere[] = "p.ptrcpf = '{$post['bencpf']}'";
    		if($post['bensemestre']){
    			$arWhere[] = "p.ptrsemestre = '{$post['bensemestre']}'";
    		}
    	}else
    		if($post['semestre']){
    		$arWhere[] = "p.ptrano = {$post['ano']} and ptrsemestre = '{$post['semestre']}' and ptrcpf = '{$post['cpf']}'";
    	}else if($post['ptrid_itens']){
    		$arWhere[] = "v.ptrid = {$post['ptrid_itens']}";
    	}
    	if($ativo){
    		$arWhere[] = "ptvstatus = 'A'";
    	}else{
    		$arWhere[] = "ptvstatus != 'A'";
    	}
    	
    	$sql = "select v.*, ifenome, ifcnome from maismedicos.planotrabalho_visitaprogramada v
    		join maismedicos.planotrabalho p on p.ptrid = v.ptrid
    		left join maismedicos.ifes u on u.ifeid = v.ifeid
    		left join maismedicos.ifes_campi c on c.ifcid = v.ifcid
    		".(is_array($arWhere) ? 'where  '.implode(' and ', $arWhere) : '');
    	$arVisita = $this->carregar($sql);
    	 
    	if($arVisita){
    		if($ini){
    		$html .= '<table align="center" class="tabela" id="dados_visita" cellpadding="3" cellspacing="1" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;">
					<thead>
					<tr class="subtitulocentro"><td colspan="7">Visita Programada</td></tr>
					<tr>
						<th>Instituição</th>
						<th>Campus</th>
						<th align="center">Período Previsto</th>
						<th>Obs</th>
						<th>Anexo</th>
						<th align="center">Período Executado</th>
						<th>Obs Execução</th>
						<th>Anexo Execução</th>
						<th></th>
					</tr>
					</thead>
						<tbody>';
    		}
    		if(!$ativo){
    			$html .= '<tr><td class="subtitulocentro" colspan="8">Visitas excluídas</td></tr>';
    		}
    		foreach($arVisita as $key => $visita){
    			if(!$ativo){
    				$marca = "color:gray;font-style:italic;";
    			}else{
    				$marca = ($post['semestre'] && $post['ptrid_itens']==$visita['ptrid']) ? 'background:#C0D9D9' : '';
    			}
    			$html .= '
    				    <tr id="tr_visita_ '.$key.'" style="'.$marca.'">
    				    		<td width="10%" valign="top" align="left">'.$visita['ifenome'].'</td>
    				    		<td width="10%" valign="top" align="left">'.$visita['ifcnome'].'</td>
    				    				<td width="12%" valign="top" align="center">
							                 '.formata_data($visita['ptvdtprevini']).'
    								                 <br/>até<br/>
							                 '.formata_data($visita['ptvdtprevfim']).'
    								                 </td><td width="21%" valign="top" align="left">
							                 '.(trim($visita['ptvobs']) ? $visita['ptvobs'] : '-').'</td><td width="5%" valign="top">';
    			 
    			if($visita['arqid']){
    				$html .= '<center><img src="../imagens/clipe.gif" id="'.$visita['arqid'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
    			}else{
    				$html .= '<center>-</center>';
    			}
    	
    			$html .= '</td><td width="12%" valign="top" align="center">
    					'.($visita['ptvdtexecini'] ? formata_data($visita['ptvdtexecini']).'<br/>até<br/>'.formata_data($visita['ptvdtexecfim']) : '-').'</td>
						<td width="20%" valign="top" align="left">'.($visita['ptvobsexecutado'] ? $visita['ptvobsexecutado'] : '-').'</td>
						<td width="5%" valign="top" align="left">';
    	
    			if($visita['arqidexecutado']){
    				$html .= '<center><img src="../imagens/clipe.gif" id="'.$visita['arqidexecutado'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
    			}else{
    				$html .= '<center>-</center>';
    			}
    			$html .= '</td><td>';
    			if(!$ativo){
    				$html .= '<img src="../imagens/garbage_little.png" title="Excluído" alt="Excluído" width="35"/>';
    			}else
    			if($visita['ptvdtenvioexec']){
    				$html .= '<img src="../imagens/check_checklist.png" title="Executado" alt="Executado" />';
    			}else{
    				$html .= '<img src="../imagens/excluir_2.gif" title="Não Executado" alt="Não Executado" />';
    			}
    			$html .= '</td></tr>';
    		}
    		if($fim){
    		$html .= '</tbody></table>';
    		}
    		
    		return $html;
    	}
    }
    
    public function montaDetalheAtividade($post = null, $ativo = true, $ini = true, $fim = true)
    {
    	if($post['bencpf']){
    		$arWhere[] = "p.ptrcpf = '{$post['bencpf']}'";
    		if($post['bensemestre']){
    			$arWhere[] = "p.ptrsemestre = '{$post['bensemestre']}'";
    		}
    	}else
    		if($post['semestre']){
    		$arWhere[] = "p.ptrano = {$post['ano']} and ptrsemestre = '{$post['semestre']}' and ptrcpf = '{$post['cpf']}'";
    	}else if($post['ptrid_itens']){
    		$arWhere[] = "v.ptrid = {$post['ptrid_itens']}";
    	
    	}
    	
    	if($ativo){
    		$arWhere[] = "ptastatus = 'A'";
    	}else{
    		$arWhere[] = "ptastatus != 'A'";
    	}
    	
    	$sql = "select v.*, ifenome, ifcnome from maismedicos.planotrabalho_atividadeacompanhamento v
    			join maismedicos.planotrabalho p on p.ptrid = v.ptrid
    	left join maismedicos.ifes u on u.ifeid = v.ifeid
    	left join maismedicos.ifes_campi c on c.ifcid = v.ifcid
    	".(is_array($arWhere) ? ' where  '.implode(' and ', $arWhere) : '');
    	//echo '<pre>';var_dump($sql);die;
    	//ver($sql);
    	$arAtividade = $this->carregar($sql);
    	
    	if($arAtividade){
    		if($ini){
	    		$html .= '<table class="tabela" id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;" align="center">
						<thead>
						<tr class="subtitulocentro"><td colspan="9">Atividades de Acompanhamento</td></tr>
							<tr>
							<th>Instituição</th>
							<th>Campus</th>
							<th>Atividade</th>
							<th align="center">Data Prevista</th>
							<th>Tipo</th>
							<th>Descrição</th>
							<th>Anexo</th>
	    			<th align="center">Data Execução</th>
	    			<th>Obs Execução</th>
	    				<th>Anexo Execução</th>
	    				<th></th></tr></thead><tbody>';
    		}
    		if(!$ativo){
    			$html .= '<tr><td class="subtitulocentro" colspan="8">Atividades excluídas</td></tr>';
    		}
    		foreach($arAtividade as $key => $atividade){
    			if(!$ativo){
    				$marca = "color:gray;font-style:italic;";
    			}else{
    				$marca = ($post['semestre'] && $post['ptrid_itens']==$atividade['ptrid']) ? 'background:#C0D9D9' : '';
    			}
    			$html .= '
    							<tr id="tr_atividade_'.$key.'" style="'.$marca.'">
							            <td width="8%" valign="top" align="left">'.$atividade['ifenome'].'</td>
							            <td width="8%" valign="top" align="left">'.$atividade['ifcnome'].'</td>
							            <td width="8%" valign="top" align="left">'.$atividade['ptanome'].'</td>
    								            		<td width="10%" valign="top" align="center">'.($atividade['ptadtprevisto'] ? formata_data($atividade['ptadtprevisto']) : '-').'</td>
							            <td width="7%" valign="top" align="left">'.($atividade['ptatipo'] ? $atividade['ptatipo'] : '-').'</td>
    								            		<td width="18%" valign="top" align="left">'.($atividade['ptadsc'] ? $atividade['ptadsc'] : '-').'</td>
    								            			<td width="5%" valign="top">';
    				
    			if($atividade['arqid']){
    				$html .= '<center><img src="../imagens/clipe.gif" id="'.$atividade['arqid'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
    				$marca = '';
    			}else{
    				$html .= '	<center>-</center>';
    			}
    			$html .= '</td><td width="10%" valign="top" align="center">'.($atividade['ptadtexecutado'] ? formata_data($atividade['ptadtexecutado']) : $atividade['ptadtexecutado']).'</td>
    					<td width="16%" valign="top">'.($atividade['ptaobsexecutado'] ? $atividade['ptaobsexecutado'] : '-').'</td><td width="5%" valign="top">';
    	
    			if($atividade['arqidexecutado']){
    				$html .= '<center><img src="../imagens/clipe.gif" id="'.$atividade['arqidexecutado'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
    			}else{
    				$html .= '<center>-</center>';
    			}
    			$html .= '</td><td>';
    			if(!$ativo){
    				$html .= '<img src="../imagens/garbage_little.png" title="Excluído" alt="Excluído" width="35"/>';
    			}else
    			if($atividade['ptadtenvioexec']){
    				$html .= '<img src="../imagens/check_checklist.png" title="Executado" />';
    			}else{
    				$html .= '<img src="../imagens/excluir_2.gif" title="Executado" />';
    			}
    			$html .= '</td></tr>';
    		}
    		if($fim){
    		$html .= '</tbody></table>';
    		}
    	}
    	
    	return $html;
    }
    
    public function recuperaPlanosBeneficiarios($post = null)
    {
    	if($post['beneficiario']){
    		$arWhere[] = "ben.bencpf = '{$post['beneficiario']}'";
    	}
    	if($post['semestre']){
    		$arSem = explode('_', $post['semestre']);
    		$arWhere[] = "ptr.ptrsemestre = '{$arSem[1]}' and ptr.ptrano = '{$arSem[0]}'";
    	}
    	$sql = "select distinct
    				'<img src=\"../imagens/lupa_grafico.gif\" class=\"btnDetalhe\" id=\"' || ben.bencpf || '_' || ptr.ptrsemestre || '\" style=\"cursor:pointer\" />' as acao,
    				ben.bencpf,
    				ben.bennome,
    				ptr.ptrsemestre,
    				sum((select count(*) from maismedicos.planotrabalho_visitaprogramada ptv where ptvstatus = 'A' and ptv.ptrid = ptr.ptrid)) as visitas, 
    				sum((select count(*) from maismedicos.planotrabalho_atividadeacompanhamento pta where ptastatus = 'A' and pta.ptrid = ptr.ptrid)) as atividades 
    			from maismedicos.pagamento_beneficiario ben
    			left join {$this->stNomeTabela} ptr on ptr.ptrcpf = ben.bencpf
    				and ptr.ptrstatus = 'A'
    			where benstatus = 'A'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
    			group by ben.bencpf, ben.bennome, ptr.ptrsemestre
    			order by ben.bennome, ptr.ptrsemestre";
    	//ver($sql, d);
    	//	echo $sql;
    	return $this->carregar($sql);
    }
    
    public function verificaBancoAgencia()
    {
    	$benid = $this->cadastraBeneficiario();
    	//var_dump($benid);echo 'aqui';
    	$sql = "select benid, bncid, benagencia from maismedicos.pagamento_beneficiario where benid = {$benid}";
    	return $this->pegaLinha($sql);
    }
    
    public function salvaDadosBancariosBeneficiario($post)
    {
    	$post['bendatanascimento'] = formata_data_sql($post['bendatanascimento']);
    	$sql = "update maismedicos.pagamento_beneficiario set 
    				bncid = '{$post['bncid']}',
    				benagencia = '{$post['benagencia']}',
    				bennomemae = '{$post['bennomemae']}',
    				bendatanascimento = '{$post['bendatanascimento']}'
    			where benid = {$post['benid']};";
    	
    	$this->executar($sql);
    	$this->commit();
    	return true;
    }
    
}