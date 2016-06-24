<?php
// Verifica o tipo da situação para realizar o filtro

class funcoesProjetoFabrica{

    function montaListaArray($dados,$cabecalho="",$perpage,$pages,$soma,$alinha,$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista", $totalizador = null) {
		// este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
		//$sql = Texto - sql que vai gerar a lista
		//$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
		//Parmetros de paginao
		//$perpage = Numrico - Registros por pgina
		//$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
		// $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
		// $ordem = alinhamento dos Tétulos (left, rigth, center)
		//$html = passa-se um array com os respesctivos HTML's que seram substituidos pelos campos do resultado,
		//		  sendo que onde se quiser colocar o valor vindo do sql deve-se escrever {campo[1]},{campo[2]},... "comeáando de {campo[1]}"
		//		  EX.: <a href="javascript:void(0);" id="campo_popup_checkbox_{campo[0]}_1" onclick="{campo[3]}">{campo[1]} - {campo[2]}</a>
		//Registro Atual (instanciado na chamada)
		/**
		$arrayDeTiposParaOrdenacao = Vetor contendo campos e seus respectivos tipos especiais, permitindo a correta ordenação dos mesmos.
		Não há necessidade de passar todos os campos já presentes no vetor(array) de dados, mas somente aqueles que são diferentes do 
		   tipo textual (string).   
		Para um dado array contendo todos os dados da lista:  
			...
			$dados_array[$posicaoNoLoop] = array("acao" 		=> $acao						  ,   
												 "num_processo" => $val['prcnumsidoc']		 ." " ,   
												 "interessado" 	=> $val['prcnomeinteressado']." " ,  
												 "prioridade" 	=> $val['prioridade']			  ,  
												 "data" 		=> $val['dataultimocadastro']	  ,  
												 "dttramite" 	=> $val['dttramite']			  ,  
												 "coord" 		=> $val['coodsc']				  ,  
												 "situacao" 	=> $val['esddsc']
								   		   );
			}
			...
			Passar também o parametro tal como (somente campos diferentes de 'string'):  
			...
			$arrayDeTiposParaOrdenacao = array();
			$arrayDeTiposParaOrdenacao[] = array( "data"   		 => "date"    );
			$arrayDeTiposParaOrdenacao[] = array( "num_processo" => "integer" );
			...
		 */
		
            if ($_REQUEST['numero']=='') 
			$numero = 1; 
		else 
			$numero = intval($_REQUEST['numero']);
		//Controla o Order by
		if ($_REQUEST['ordemlista']<>'') {

			$dados = $this->criarColunasDeOrdenacaoEmArray($dados,$arrayDeTiposParaOrdenacao,$_REQUEST['ordemlista']);
			$campoParaOrdenacao = $_REQUEST['ordemlista'];  
//			dbg($campoParaOrdenacao,1);  		
			
			if($arrayDeTiposParaOrdenacao){
				foreach( $arrayDeTiposParaOrdenacao as $linhaParaOrdenacao ) {
					foreach( $linhaParaOrdenacao as $chaveParaOrdenacao => $tipoParaOrdenacao ) {
						if( $chaveParaOrdenacao == $campoParaOrdenacao ) {
							$campoParaOrdenacao .= "_campoextra";			
						}
					}
				}
			}
			
			if ($_REQUEST['ordemlistadir'] <> 'DESC') {
				$ordemlistadir = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir = 'DESC'; 
				$ordemlistadir2 = 'ASC';
			}
		
			// 	Obter uma lista de colunas
			foreach ($dados as $key => $row) {
				$row = ereg_replace("[^a-zA-Z0-9_]", "", strtr($row[($campoParaOrdenacao)], "áááááááááááááááááááááááááá", "aaaaeeiooouucAAAAEEIOOOUUC"));
		    	$crt[$key]  = $row;
			}
			
			switch($ordemlistadir) {
				case 'ASC':
					array_multisort($crt, SORT_ASC , $dados);  
					break;
				case 'DESC':
					array_multisort($crt, SORT_DESC, $dados); 				 
					break;
			}
		}
		
	    $RS = $dados;
		$nlinhas = count($RS);
		if (! $RS) 
			$nl = 0; 
		else 
			$nl=$nlinhas;
		if (($numero+$perpage)>$nlinhas) 
			$reg_fim = $nlinhas; 
		else 
			$reg_fim = $numero+$perpage-1;
		if ($nl>0){
			
			$ordenador = array_keys($RS[0]);
			
			$total_reg = $nlinhas;
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			//monta o formulario da lista mantendo os parametros atuais da pgina

			if($formName){
				print '<form name="'.$formName.'" method="post">
						<input type="Hidden" name="numero" value="" />
						<input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/>
						<input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
			}
			
			foreach($_POST as $k=>$v){
				if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero')
					if ( is_array($v) ){
						while ($val = current($v)){
							print '<input type="hidden" name="'.$k.'[]" value=\'' . $val . '\'/>';
							next($v);
						}
					}else{
						print '<input type="hidden" name="'.$k.'" value=\'' . $v . '\'/>';
					} 
			}
			
			if($formName) print '</form>';
			//Monta Cabealho
			
			$campoOrdenacaoOriginal = $_REQUEST['ordemlista']; 
			//$posicao_ordenador_original = $camposOrdenacaoSubstituicao['posicao_ordenador_original'];
			
			if ( $cabecalho === null ) {
	
			}else if(is_array($cabecalho)){
				print '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if ($campoParaOrdenacao==($ordenador[$i]) || $campoOrdenacaoOriginal==($ordenador[$i]) ) {  
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					if($formName) print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($ordenador[$i]).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
					else print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($campoParaOrdenacao==($i+1) ) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						
						if($formName) print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						else  print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">'.$imgordem.'<strong>'.$k.'</strong></label>';
						
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();
			
			//Recebe padrÃ£o de substituiÃ§Ã£o de HTML
			$search = array();		
			for ($i=($numero-1);$i<$reg_fim;$i++)
			{
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
		
				if ( count($RS[$i]) != count($search) ):
					for ($z=1; $z <= count($RS[$i]); $z++){
						$search[$z] = '{campo['.$z.']}';
					}
				endif;
				$col = 0;
				foreach($RS[$i] as $k=>$v) {
					if ( $col > count($cabecalho)-1 && count($cabecalho) ) break;
								
					if (is_numeric($v)){
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')) {
							$v = number_format($v, 2, ',', '.'); 
							if (!$tipovl['0'.$c]) {
								$coluna = array('0'.$c => 'vl'); 
								$tipovl = array_merge($totais, $coluna);
							}else{ 
								$tipovl['0'.$c] = 'vl';
							}	
						}
						if ($v<0) 
							print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v).')'; 
						else 
							print '<td align="right" style="color:#0066cc;" title="'.$cabecalho[$c].'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);
							
						print ('<br>'.$totais[$c]);
						
					}else{ 
						print '<td title="'.$cabecalho[$c].'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);
					}	
					print '</td>';
					
					$c = $c + 1;
					$col++;
				}
				print '</tr>';
			}
	
			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					print '<td align="right" title="'.$cabecalho[$i].'">';
	
					if ($i==0) print 'Totais:   ';
					if (is_numeric($totais['0'.$i])) print number_format($totais['0'.$i], 2, ',', '.'); else print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			}
	
			print '</table>';
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de SS/OS: ' . $totalizador . '</b></td><td>';
	
			include APPRAIZ."includes/paginacao.inc";
			print '</td></tr></table>';
			
			if($formName){
				print '<script language="JavaScript">';
				print 'function ordena(ordem, direcao) {';
				print 	'document.'.$formName.'.ordemlista.value=ordem;';
				print	'document.'.$formName.'.ordemlistadir.value=direcao;';
				print 	'document.'.$formName.'.submit()';
				print 	'} ';
				print 'function pagina(numero) {';
				print 	'document.'.$formName.'.numero.value=numero;';
				print	'document.'.$formName.'.submit();';
				print	'} ';
				print '</script>';
			}
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

    function excluirAnexoProjeto() {
        global $db;

        $prjid = $_REQUEST['prjid'];
        $arqid = $_REQUEST['arqid'];

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $campos = array(
            "tapid" => null,
            "prjid" => null,
            "anpdsc" => null,
            "anpstatus" => null,
            "anpdtinclusao" => null
        );

        $file = new FilesSimec("anexoprojeto", $campos, "fabrica");
        $file->setRemoveUpload($arqid);

        include APPRAIZ . "fabrica/classes/Projeto.class.inc";

        $projeto = new Projeto();
        $projeto->showAnexosProjeto($prjid);
    }

    function downwloadArquivo() {

        $arqid = $_REQUEST['arqid'];

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $campos = array(
            "tapid" => null,
            "prjid" => null,
            "anpdsc" => null,
            "anpstatus" => null,
            "anpdtinclusao" => null
        );

        $file = new FilesSimec("anexoprojeto", $campos, "fabrica");
        $file->getDownloadArquivo($arqid);
    }

    function recuperarProdutosProjeto($prjid = null) {
        global $db;

        if (!$prjid) {
            return array();
        }

        $sql = sprintf("select distinct
						prd.prdid as codigo,
						prd.prddsc as descricao
					from
						fabrica.projetoproduto prjp
					inner join
						fabrica.produto prd ON prjp.prdid = prd.prdid
					where
						prjp.prjid = %d
					and
						prd.prdstatus = 'A'", (int) $prjid);

        return $db->carregar($sql);
    }

    function deletarProdutosProjeto($prjid) {
        global $db;
        $sql = sprintf("delete from fabrica.projetoproduto where prjid = %d", (int) $prjid);
        $db->executar($sql);
        $db->commit();
    }

    function inserirProdutosProjeto($prjid, $arrDados = null) {
        global $db;

        if ($arrDados && is_array($arrDados)) {

            foreach ($arrDados as $produto) {
                $sql .= "insert into fabrica.projetoproduto (prdid,prjid) values ($produto,$prjid);";
            }

            $db->executar($sql);
            $db->commit();
        }
    }

    function deletarModulosProjeto() {
        global $db;

        $mdpid = $_REQUEST['mdpid'];

        $sql = sprintf("select
						mdl.mdpid
					from
						fabrica.moduloprojeto mdl
					inner join
						fabrica.analisesolicitacao ana ON ana.mdpid = mdl.mdpid
					where
						mdl.mdpid = %d", $mdpid);
        $count = $db->pegaUm($sql);

        if ($count) {
            echo "Esse módulo esta vinculado a uma análise!";
            return false;
        } else {
            $sql = sprintf("update fabrica.moduloprojeto set mdpstatus = 'I' where mdpid = %d", (int) $mdpid);
            $db->executar($sql);
            $db->commit();
            return true;
        }
    }

    function inserirModulosProjeto($prjid, $arrDados = null) {
        global $db;

        if ($arrDados && is_array($arrDados)) {

            foreach ($arrDados as $mdpid => $modulo) {

                if (is_int($mdpid)) {
                    $sql .= "update fabrica.moduloprojeto set mdpdsc = '" . trim($modulo) . "' where mdpid = $mdpid;";
                } else {
                    $sql .= "insert into fabrica.moduloprojeto (prjid,mdpdsc,mdpstatus) values ($prjid,'" . trim($modulo) . "','A');";
                }
            }

            $db->executar($sql);
            $db->commit();
        }
    }

    function salvarRegistroEntidade() {
        global $db;
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        $entidade = new Entidades();
        $entidade->carregarEntidade($_REQUEST);
        $entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
        $entidade->salvar();
        return $entidade->getEntId();
    }

    function exibirDemandasExecucao() {
        global $db;
        $sql = "select distinct
				'<span class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\" >' || os.odsid || '</span>'as num,
				'<div onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'
				||
				--'<span title=\"\" class=\"link\" onmouseover=\"SuperTitleAjax(\'fabrica.php?modulo=principal/painelAcompanhamento&acao=A&requisicaoAjax=listarDesciplinasOrdemServico&&ansid=' || s.ansid || '\') \" >' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				'<span title=\"\" class=\"link\">' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				||
				'</div>' as disciplina,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||coalesce((select siddescricao from demandas.sistemadetalhe sid where sid.sidid = cts.sidid),'N/A')||'</div>' as sistema,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||to_char(odsdtprevinicio,'DD/MM/YYYY HH24:MI')||'</div>' as inicio,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||to_char(odsdtprevtermino,'DD/MM/YYYY HH24:MI')||'</div>' as termino
			from fabrica.ordemservico os
			inner join fabrica.analisesolicitacao s ON s.scsid = os.scsid
			left join fabrica.solicitacaoservico cts ON cts.scsid = s.scsid
			inner join fabrica.servicofaseproduto sfp ON sfp.ansid = s.ansid and sfp.tpeid = os.tpeid
			inner join fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sfp.fdpid
			inner join fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
			inner join fabrica.produto p ON p.prdid = fdp.prdid
			inner join
				workflow.documento wok ON wok.docid = os.docid
			where
				wok.esdid = " . WF_ESTADO_OS_EXECUCAO . "
			group by
				os.odsid,
				os.odsdtprevinicio,
				os.odsdtprevtermino,
				cts.sidid,
				s.scsid,
				s.ansid,
				os.odsid";

        $cabecalho = array("NãoS", "Disciplina/Fase/Artefato", "Sistema", "Data de Início", "Data de Término");
        $db->monta_lista_simples($sql, $cabecalho, 100, 50, "N", "100%");
    }

    function exibirSolicitacoesServico() {
        global $db;

        $sql = "select distinct
				'<center><a href=\"javascript: exibirSolicitacao(\'' || scs.scsid || '\',\'' || s.ansid || '\')\">' || scs.scsid || '</a></center>' as nss,
				'<span class=\"link\" onclick=\"exibirSolicitacao(\'' || scs.scsid || '\',\'' || s.ansid || '\')\" >' || (select usunome from seguranca.usuario where usucpf = scs.usucpfrequisitante) || '</span>' as usucpfrequisitante,
				coalesce((select siddescricao from demandas.sistemadetalhe sid where sid.sidid = scs.sidid),'N/A') as sistema,
				'<div class=\"link\" onclick=\"exibirSolicitacao(\'' || scs.scsid || '\',\'' || s.ansid || '\')\">'
				||
				--'<span title=\"\" class=\"link\" onmouseover=\"SuperTitleAjax(\'fabrica.php?modulo=principal/painelAcompanhamento&acao=A&requisicaoAjax=listarDesciplinasOrdemServico&&ansid=' || s.ansid || '\') \" >' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				'<span title=\"\" class=\"link\" >' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				||
				'</div>' as disciplina,
				'<div class=\"link\" onclick=\"exibirSolicitacao(\'' || scs.scsid || '\',\'' || s.ansid || '\')\">'||to_char(ansprevinicio,'DD/MM/YYYY HH24:MI')||'</div>' as ansprevinicio,
				'<div class=\"link\" onclick=\"exibirSolicitacao(\'' || scs.scsid || '\',\'' || s.ansid || '\')\">'||to_char(ansprevtermino,'DD/MM/YYYY HH24:MI')||'</div>' as ansprevtermino
			from fabrica.solicitacaoservico scs --ON scs.scsid = os.scsid
			inner join fabrica.analisesolicitacao s ON s.scsid = scs.scsid
			inner join workflow.documento doc ON doc.docid = scs.docid

			where
				scsstatus = 'A' AND esdid = '" . WF_ESTADO_DETALHAMENTO . "'
			group by
				scs.scsid,
				 s.ansid,
				 scs.usucpfrequisitante,
				 ansprevinicio,
				 ansprevtermino,
				 scs.sidid";

        #echo "<pre>";
        #exit($sql);
       $cabecalho = array("Nº SS", "Requisitante", "Sistema", "Disciplina/Fase/Artefato", "Data de Início", "Data de Término");

        $db->monta_lista_simples($sql, $cabecalho, 100, 50, "N", "100%");
    }

    function exibirDemandasAprovadas() {
        global $db;

        $sql = "select distinct
				'<span class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\" >' || os.odsid || '</span>'as num,
				'<div onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'
				||
				--'<span title=\"\" class=\"link\" onmouseover=\"SuperTitleAjax(\'fabrica.php?modulo=principal/painelAcompanhamento&acao=A&requisicaoAjax=listarDesciplinasOrdemServico&&ansid=' || s.ansid || '\') \" >' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				'<span title=\"\" class=\"link\" >' || (select count(fdpid) from fabrica.servicofaseproduto abc where abc.ansid = s.ansid and abc.tpeid=1) || ' artefato(s)</span>'
				||
				'</div>' as disciplina,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||coalesce((select siddescricao from demandas.sistemadetalhe sid where sid.sidid = cts.sidid),'N/A')||'</div>' as sistema,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||to_char(odsdtprevinicio,'DD/MM/YYYY HH24:MI')||'</div>' as inicio,
				'<div class=\"link\" onclick=\"abrirOS(\'' || s.scsid || '\',\'' || s.ansid || '\',\'' || os.odsid || '\')\">'||to_char(odsdtprevtermino,'DD/MM/YYYY HH24:MI')||'</div>' as termino
			from fabrica.ordemservico os
			inner join fabrica.analisesolicitacao s ON s.scsid = os.scsid
			left join fabrica.solicitacaoservico cts ON cts.scsid = s.scsid
			inner join fabrica.servicofaseproduto sfp ON sfp.ansid = s.ansid and sfp.tpeid = os.tpeid
			inner join fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sfp.fdpid
			inner join fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
			inner join fabrica.produto p ON p.prdid = fdp.prdid
			inner join
				workflow.documento wok ON wok.docid = os.docid
			where
				wok.esdid = " . WF_ESTADO_OS_PENDENTE . "
			group by
				os.odsid,
				os.odsdtprevinicio,
				os.odsdtprevtermino,
				cts.sidid,
				s.scsid,
				s.ansid,
				os.odsid";

        $cabecalho = array("NãoS", "Disciplina/Fase/Artefato", "Sistema", "Data de Início", "Data de Término");
        $db->monta_lista_simples($sql, $cabecalho, 100, 50, "N", "100%");
    }

    function exibirAtestarDemandas() {
        global $db;
        $sql = "select distinct
				'<div><input type=\"radio\" ' || (CASE WHEN scsatesto IS TRUE THEN 'checked=\"checked\"' ELSE '' END) || ' onclick=\"atestarSolicitacao(\'' || scs.scsid || '\')\" name=\"rdb_atestar_' || scs.scsid || '\" value=\"sim\" />Sim <input ' || (CASE WHEN scsatesto IS FALSE THEN 'checked=\"checked\"' ELSE '' END) || ' onclick=\"atestarSolicitacao(\'' || scs.scsid || '\')\" type=\"radio\" name=\"rdb_atestar_' || scs.scsid  || '\" value=\"nao\" />Não</div>' as atestar,
				'<center><a href=\"javascript: exibirSolicitacao(\'' || scs.scsid || '\',\'' || ans.ansid || '\')\">' || scs.scsid || '</a></center>' as nss,
				coalesce((select siddescricao from demandas.sistemadetalhe sid where sid.sidid = scs.sidid),'N/A') as sistema,
				tpsdsc,
				to_char(ansprevinicio,'DD/MM/YYYY HH24:MI') as ansprevinicio,
				to_char(ansprevtermino,'DD/MM/YYYY HH24:MI') as ansprevtermino
			from
				fabrica.ordemservico ord
			inner join
				fabrica.solicitacaoservico scs ON scs.scsid = ord.scsid
			inner join
				fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
			inner join
				fabrica.tiposervico tps ON tps.tpsid = ans.tpsid
			where
				scsstatus = 'A'";

        $cabecalho = array("Atestar", "Nº SS", "Sistema", "Tipo de Servião", "Data de Início", "Data de Término");
        $db->monta_lista_simples($sql, $cabecalho, 100, 50, "N", "100%");
    }

    function exibirFinanceiroSS($tpeid, $tpEstimadaDetalhada = null) {
        global $db, $flagD, $flagF;

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoSS = "
						AND (
						     (
						     to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(ss.dataabertura, 'YYYY-MM-DD') and to_char(ans.ansprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(ss.dataabertura, 'YYYY-MM-DD') and to_char(ans.ansprevtermino, 'YYYY-MM-DD')
						     )
						   )
					  ";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoOS = "
						AND (
						     (
						     to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(os.odsdtprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
						     )
						   )
					  ";

            if ($tpeid == 1) { // empresa 1
                $sql = "SELECT distinct ans.ansid
					FROM fabrica.ordemservico os
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
					LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
					where os.tosid in (1)
					$andPeriodoOS
					order by 1";
            } else {
                $sql = "SELECT distinct ans.ansid
					FROM fabrica.ordemservico os
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
					LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
					where os.tosid in (2,3)
					$andPeriodoOS
					order by 1";
            }
            $filtraSSAnsid = $db->carregarColuna($sql);
            if ($filtraSSAnsid)
                $filtraSSAnsid = " AND ans.ansid in (" . implode(',', $filtraSSAnsid) . ")";
        }


        //246,247,248,249,250,252 = estimadas
        //253 = finalizada - detalhada
        //251 = cancelada - estimada
        $campoQtdOS = "os.odsqtdpfestimada";
        $andEsdidSS = " and esdid is null";
        if ($tpEstimadaDetalhada == 1) {
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidSS = " and esdid in (246,247,248,249,250,252)";
        } elseif ($tpEstimadaDetalhada == 2) {
            $campoQtdOS = "os.odsqtdpfdetalhada";
            $andEsdidSS = " and esdid in (253)";
        } elseif ($tpEstimadaDetalhada == 3) {
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidSS = " and esdid in (251,300)";
        }


        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false";
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true";
        }
        $vpcvalor = $db->pegaUm($sql);


         //lista as situaÃ§Ãµes
        $sql = "SELECT esdid, esddsc FROM workflow.estadodocumento where esdstatus = 'A'
			and tpdid = " . WORKFLOW_SOLICITACAO_SERVICO . "
			$andEsdidSS
			order by esdordem";
        $situacao = $db->carregar($sql);

        if ($situacao) {
            $i = 0;
            foreach ($situacao as $s) {
                 //echo "-------- ".$s['esddsc']." -------------<br>";
                // CondiÃ§Ã£o criada para apresentar apenas SS em analise
                if ($s['esdid'] == 246) {
                    $andTos = " and ss.scsstatus='A'  ";
                } else {
                    $andTos = " and os.tosid in (1)";
                }

                  // CondiÃ§Ã£o criada para apresentar apenas SS em finaliza e cancelada com custo
                if ($s['esdid'] == 253 || $s['esdid'] == 300) {
                    $andTipoSituacao = $flagF;
                } else {
                    $andTipoSituacao = $flagD;
                    ;
                }


                //lista analisesolicitacao
                if ($tpeid == 1) { // empresa 1
                    $sql = "SELECT distinct ans.ansid
						FROM fabrica.analisesolicitacao ans
						LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
						LEFT JOIN workflow.documento d  ON ss.docid = d.docid --and os.tpeid = 2
						LEFT JOIN fabrica.ordemservico os ON ans.scsid = os.scsid
						where d.esdid = {$s['esdid']}
						--and (os.tosid in (1) OR os.tosid is null)
						--and os.tosid in (1)
                                                $andTos
                                                $andTipoSituacao
						$andPeriodoSS
						$filtraSSAnsid";
                } else { // empresa 2
                    $sql = "SELECT distinct ans.ansid
						FROM fabrica.analisesolicitacao ans
						LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
						LEFT JOIN workflow.documento d  ON ss.docid = d.docid --and os.tpeid = 2
						LEFT JOIN fabrica.ordemservico os ON ans.scsid = os.scsid
						where d.esdid = {$s['esdid']}
						and os.tosid in (2,3)
						$andPeriodoSS
						$filtraSSAnsid";
                }
                //dbg($sql);
                $solicitacao = $db->carregar($sql);

                $qtdpfTotal = 0;
                $porcentoPf = 0;
                $valorTotal = 0;
                if ($solicitacao) {

                    foreach ($solicitacao as $ss) {

                        $porcentoPf = 0;
                        //$valorTotal = 0;
                        //pega odsqtdpfestimada das OSs
                        if ($tpeid == 1) { // empresa 1
                            $sql = "SELECT COALESCE(sum($campoQtdOS),0) as qtdpf
								FROM fabrica.ordemservico os
								LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
								LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
								where ans.ansid = {$ss['ansid']}
								and os.tosid in (1)
								$andPeriodoOS";
                        } else {
                            $sql = "SELECT COALESCE(sum($campoQtdOS),0) as qtdpf
								FROM fabrica.ordemservico os
								LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
								LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
								where ans.ansid = {$ss['ansid']}
								and os.tosid in (2,3)
								$andPeriodoOS";
                        }
                        $qtdpf = $db->pegaUm($sql);
                        $qtdpfTotal = $qtdpfTotal + $qtdpf;
                        $porcentoPf = 100;
                        $valorPf = $vpcvalor / 100;
                        $valorPfFinal = $porcentoPf * $valorPf;
                        $valorTotal = $valorTotal + ($qtdpf * $valorPfFinal);
                    }
                }

                $link = '';
                $linhaAdd = '';

                if (($solicitacao ? count($solicitacao) : 0) > 0) {
                    $link = '<a href="javascript:void(0);" onclick="montaSubLista(\'' . $s['esdid'] . '\',\'' . $tpEstimadaDetalhada . '\')"><img id="img_mais_' . $s['esdid'] . '" src="../imagens/mais.gif" border=0></a> <a href="javascript:void(0);" onclick="desmontaSubLista(\'' . $s['esdid'] . '\')"><img id="img_menos_' . $s['esdid'] . '" src="../imagens/menos.gif" border=0 style="display:none"></a> ';
                    $linhaAdd = '</tr><tr style="background-color:#F7F7F7"><td colspan=5 style="padding-left:20px;" id="td_' . $s['esdid'] . '" ></td></tr>';
                }

                $dados[$i] = array("situacao" => $link . $s['esddsc'],
                    "qtdSS" => ($solicitacao ? count($solicitacao) : '0'),
                    "qtdPFEstimado" => $qtdpfTotal,
                    "vlrPFEstimado" => $qtdpfTotal,
                    "qtdPFDetalhado" => $qtdpfTotal,
                    "vlrPFDetalhado" => $valorTotal,
                    "valor" => $valorTotal,
                    "linhaAdd" => $linhaAdd);

                //echo "-------- FIM ".$s['esddsc']." -------------<br>";
                //echo "<br>";

                $i++;
            }
        }
         $cabecalho = array("Situação", "Qtd. SS/OS", "Qtd. PF Estimado", "Valor Estimado(R$)", "Qtd. PF Detalhado", "Valor PF Detalhado (R$)", "Valor a Pagar (R$)");
        $db->monta_lista_array($dados, $cabecalho, 100, 10, 'S', 'center', '', '');
    }

    function exibirFinanceiroSubListaSS($tpeid, $tpEstimadaDetalhada = null, $esdid) {
        global $db;

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoSS = "
						AND (
						     (
						     to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(ss.dataabertura, 'YYYY-MM-DD') and to_char(ans.ansprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(ss.dataabertura, 'YYYY-MM-DD') and to_char(ans.ansprevtermino, 'YYYY-MM-DD')
						     )
						   ) ";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoOS = "
						AND (
						     (
						     to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(os.odsdtprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
						     )
						   ) ";

            if ($tpeid == 1) { // empresa 1
                $sql = "SELECT distinct ans.ansid
					FROM fabrica.ordemservico os
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
					LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
					where os.tosid in (1)
					$andPeriodoOS
					order by 1";
            } else {
                $sql = "SELECT distinct ans.ansid
					FROM fabrica.ordemservico os
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
					LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
					where os.tosid in (2,3)
					$andPeriodoOS
					order by 1";
            }
            $filtraSSAnsid = $db->carregarColuna($sql);
            if ($filtraSSAnsid)
                $filtraSSAnsid = " AND ans.ansid in (" . implode(',', $filtraSSAnsid) . ")";
        }

        //246,247,248,249,250,252 = estimadas
        //251 = cancelada - estimada
        //253 = finalizada - detalhada
        $campoQtdOS = "os.odsqtdpfestimada";
        $andEsdidSS = " and esdid is null";
        if ($tpEstimadaDetalhada == 1) {
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidSS = " and esdid in (246,247,248,249,250,252)";
        } elseif ($tpEstimadaDetalhada == 2) {
            $campoQtdOS = "os.odsqtdpfdetalhada";
            $andEsdidSS = " and esdid in (253)";
        } elseif ($tpEstimadaDetalhada == 3) {
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidSS = " and esdid in (251,300)";
        }

        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false";
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true";
        }
        $vpcvalor = $db->pegaUm($sql);

        // Condição criada para apresentar apenas SS em analise
        if ($esdid == 246) {
            $andTos = " and ss.scsstatus='A'  ";
        } else {
            $andTos = " and os.tosid in (1)";
        }

        //lista as ss
        $sql = "SELECT distinct
	       '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        || '<span title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||ss.scsid||'&ansid='||ans.ansid|| '\'\">'||ss.scsid||'</span>'
                        || '</div>' as solicitacao,
			'<center>' || to_char(ss.dataabertura, 'DD/MM/YYYY') || '</center>' as dtini,
			'<center>' || to_char(ans.ansprevtermino, 'DD/MM/YYYY') || '</center>' as dtfim,
			COALESCE(sum($campoQtdOS),0) as qtd,
			COALESCE((sum($campoQtdOS) * $vpcvalor),0) as valor
			FROM fabrica.analisesolicitacao ans
			LEFT JOIN fabrica.solicitacaoservico ss ON ans.scsid = ss.scsid
			LEFT JOIN workflow.documento d  ON ss.docid = d.docid
			LEFT JOIN fabrica.ordemservico os ON ans.scsid = os.scsid
			where d.esdid = {$esdid}
			$andTos
			$andPeriodoSS
			$filtraSSAnsid
			group by solicitacao, ss.dataabertura, ans.ansprevtermino
			order by 1";

		$cabecalho = array("Nº SS", "Prev. Início", "Prev. Término", "Qtd. PF", "Valor (R$)");
        $db->monta_lista_simples($sql, $cabecalho, 100, 50, "N", "100%");
    }

    function exibirFinanceiroOS($tpeid, $tpEstimadaDetalhada = null, $tipoLista = null, $ctrid) {
        global $db, $flagD, $flagF;

        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND os.ctrid = $ctrid";
        }


        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoOS = "
						AND (
						     (
						     to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(os.odsdtprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
						     )
						   )
					  ";
        }

        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false ";

            //254,255,null = pendente, execucao e criada - estimadas
            //256,257,261,262,263,264 = detalhada
            //301,302 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidOS = " and esdid is null";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (254,255)";
                $union = "union select null, 'Criada', 1";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (256,257,261,262,263,264)";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (301,302)";
            }
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true ";
            //ver($sql);
            //272,273,274,275,276,277 = estimadas ou detalhadas
            //303 = canceladas
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,371,277,514)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (272,273,274,275,371,277,514)";
                $tosid = "3";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 4) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "3";
            }
        }
        
    	if ($_REQUEST['tipoSituacao'] == 1) {//Desenvolvimento
            $flag_OS_D = "AND d.esdid not in (303,277)";
        } elseif ($_REQUEST['tipoSituacao'] == 2) {//Utilizada
            $flag_OS_U = "AND d.esdid in (277)";
        } elseif ($_REQUEST['tipoSituacao'] == 0) {//todos
            $flag_OS_D = " ";
        }
        $vpcvalor = $db->pegaUm($sql);


        //lista as situaááes
        $sql = "SELECT esdid, esddsc, esdordem,
				(CASE
					WHEN esdid = 272 THEN 1 -- Pendente
					WHEN esdid = 273 THEN 2 -- Aguardando Contagem
					WHEN esdid = 514 THEN 3 -- Aguardando Contagem
					WHEN esdid = 274 THEN 4 -- Em Avaliação
					WHEN esdid = 275 THEN 5 -- Em Aprovação
					WHEN esdid = 276 THEN 6 -- Em Revisão
					WHEN esdid = 371 THEN 7 -- Aguardando Pagamento
					WHEN esdid = 277 THEN 8 -- Finalizada
				END)as situacao
				FROM workflow.estadodocumento where esdstatus = 'A'
			and tpdid = " . ($tpeid == 1 ? WORKFLOW_ORDEM_SERVICO : WORKFLOW_CONTAGEM_PF) . "
			$andEsdidOS
			$union
			order by situacao, esddsc";
			//ver($sql);
        $situacao = $db->carregar($sql);

        if ($situacao) {
            $i = 0;
            $qtdTotalOS = 0;
            foreach ($situacao as $s) {

                //echo "-------- ".$s['esddsc']." -------------<br>";
                // Condição criada para apresentar apenas SS em analise

            if ($_REQUEST['tipoSituacao'] == 1) {//Desenvolvimento
	            $andTipoSituacao = $flag_OS_D;
	        } elseif ($_REQUEST['tipoSituacao'] == 2) {//Utilizada
	           $andTipoSituacao = $flag_OS_U;
	        } elseif ($_REQUEST['tipoSituacao'] == 0) {//todos
	            $andTipoSituacao = " ";
	        }

                //lista analisesolicitacao
                if ($tpeid == 2) { // empresa 2
                    if (!$s['esdid']) {
                        $sql = "SELECT distinct os.odsid, os.scsid
							FROM fabrica.ordemservico os
							LEFT JOIN workflow.documento d  ON d.docid = os.docidpf
                                                        LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
                                                        LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
                                                        LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid                                                        
							where os.docidpf is null
							and os.tosid in (" . $tosid . ")
                            $AndEmpresa
                            $andTipoSituacao
							$andPeriodoOS";
                    } else {
                        $sql = "SELECT distinct os.odsid, os.scsid
							FROM fabrica.ordemservico os
							LEFT JOIN workflow.documento d  ON d.docid = os.docidpf
                                                        LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
                                                        LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
                                                        LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid                                    
							where d.esdid = {$s['esdid']}
							and os.tosid in (" . $tosid . ")
                        	$AndEmpresa
                        	$andTipoSituacao
							$andPeriodoOS";
                    }
                }
                //dbg($sql);
                $solicitacaoOS = $db->carregar($sql);


                $qtdpfTotal = 0;
                $porcentoPf = 0;
                $valorTotal = 0;
                if ($solicitacaoOS) {

                    foreach ($solicitacaoOS as $ss) {
                        $porcentoPf = 0;
                        //pega odsqtdpfestimada das OSs
                        if ($tpeid == 1) { // empresa 1
                            $sql = "SELECT COALESCE(sum($campoQtdOS),0) as qtdpf
								FROM fabrica.ordemservico os
								where os.odsid = {$ss['odsid']}
								and os.tosid in (1)";
                        } else {
                            $sql = "SELECT COALESCE(sum($campoQtdOS),0) as qtdpf
								FROM fabrica.ordemservico os
								where os.odsid = {$ss['odsid']}
								and os.tosid in (" . $tosid . ")";
                            // ver($sql);
                        }
                        $qtdpf = $db->pegaUm($sql);

                        $qtdpfTotal = $qtdpfTotal + $qtdpf;
                        $porcentoPf = 100;
                        $valorPf = $vpcvalor / 100;
                        $valorPfFinal = $porcentoPf * $valorPf;
                        $valorTotal = $valorTotal + ($qtdpf * $valorPfFinal);
                    }
                }

                $link = '';
                $linhaAdd = '';
                if (($solicitacaoOS ? count($solicitacaoOS) : 0) > 0) {
                    if (!$s['esdid'])
                        $s['esdid'] = 'x';
                    $link = '<a href="javascript:void(0);" onclick="montaSubListaOS(\'' . $s['esdid'] . '\',\'' . $tpEstimadaDetalhada . '\',\'' . $tipoLista . '\',\'' . $ctrid . '\')"><img id="img_mais_' . $tipoLista . $s['esdid'] . '" src="../imagens/mais.gif" border=0></a> <a href="javascript:void(0);" onclick="desmontaSubListaOS(\'' . $s['esdid'] . '\',\'' . $tipoLista . '\')"><img id="img_menos_' . $tipoLista . $s['esdid'] . '" src="../imagens/menos.gif" border=0 style="display:none"></a> ';
                    $linhaAdd = '</tr><tr style="background-color:#F7F7F7"><td colspan=5 style="padding-left:20px;" id="td_' . $tipoLista . $s['esdid'] . '" ></td></tr>';
                }

                $dados[$i] = array(
                    "situacao" => $link . $s['esddsc'],
                    "qtdOS" => ($solicitacaoOS ? count($solicitacaoOS) : '<center>-</center>'),
                    "qtdPF" => $qtdpfTotal <> '' ? sprintf("%01.2f",$qtdpfTotal) : ' <center> - </center> ',
                    "valor" => $valorTotal <> '' ? sprintf("%01.2f",$valorTotal) : ' <center> - </center> ',
                    "linhaAdd" => $linhaAdd);

                $qtdTotalOS += $dados[$i]['qtdOS'];
                $i++;
            }
        }

       $cabecalho = array("Situação", "Qtd. OS", "Qtd. PF", "Valor (R$)", "");
       $this->montaListaArray($dados, $cabecalho, count($dados), 50, "N", "100%", null, null, null, $qtdTotalOS);
    }

    function exibirFinanceiroSubListaOS($tpeid, $tpEstimadaDetalhada = null, $esdid = null, $ctrid) {
        global $db;

        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND os.ctrid = $ctrid";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoOS = "
						AND (
						     (
						     to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or
						     to_char(os.odsdtprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or
						     (
						     '$dtinip' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
							or
						     '$dtfimp' BETWEEN to_char(os.odsdtprevinicio, 'YYYY-MM-DD') and to_char(os.odsdtprevtermino, 'YYYY-MM-DD')
						     )
						   ) ";
         }

        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false ";

            //254,255,null = pendente, execucao e criada - estimadas
            //256,257,261,262,263,264 = detalhada
            //301,302 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidOS = " and esdid is null";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (254,255)";
                $union = "union select null, 'Criada', 1";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (256,257,261,262,263,264)";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (301,302)";
            }
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true  ";

            //272,273,274,275,276,277 = estimadas ou detalhadas
            //303 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "3";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 4) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "3";
            }
        }
        $vpcvalor = $db->pegaUm($sql);


        $arrPerfil = pegaPerfilGeral();


//permissao PERFIL_PREPOSTO, PERFIL_ESPECIALISTA_SQUADRA
        $botoesContagem = 1;
        if ((in_array(PERFIL_PREPOSTO, $arrPerfil) || in_array(PERFIL_ESPECIALISTA_SQUADRA, $arrPerfil)) && !in_array(PERFIL_SUPER_USUARIO, $arrPerfil)) {
            $botoesContagem = 0;
        }
        //lista as OS
            $sql = "SELECT distinct
                    '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN tos.tosid = " . TIPO_OS_GERAL . " THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||os.odsid||'</p>'
                              ELSE
                                   (CASE WHEN ed2.esdid != " . WF_ESTADO_CPF_CANCELADA . " AND 1=$botoesContagem THEN
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                         ELSE
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                    END )
                         END)
                     || '</div>' AS odsid,

                        '<div style=\"white-space: nowrap; color: #0066CC;\">'                        
                        || '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||os.scsid||'&ansid='||a.ansid|| '\'\">'||os.scsid||'</p>'                         
                    || '</div>' AS scsid,
                             '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN sd.sidabrev = sd.sidabrev THEN '<p title=\"' || sd.sidabrev || '\">' || substr(sd.sidabrev,0,6) ||'</p>'
                         END)
                     || '</div>' AS sidabrev,
                        

                    '<center>' || to_char(os.odsdtprevinicio, 'DD/MM/YYYY') || '</center>' as dtini,
                    '<center>' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</center>' as dtfim,
                    COALESCE($campoQtdOS,0) as qtd,
                    COALESCE(($campoQtdOS * $vpcvalor),0) as valor,
                    '<span style= \"display:none;\">'||lpad(os.odsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">' as campoOrdenar
                    FROM fabrica.ordemservico os
                    LEFT JOIN workflow.documento d  ON d.docid = os.docidpf

                        LEFT JOIN workflow.estadodocumento ed ON ed.esdid=d.esdid
                        LEFT JOIN workflow.documento d2 ON d2.docid=os.docidpf
                        LEFT JOIN workflow.estadodocumento ed2 ON ed2.esdid=d2.esdid
                        LEFT JOIN fabrica.tipoordemservico tos ON tos.tosid=os.tosid
                        LEFT JOIN fabrica.analisesolicitacao a ON a.scsid=os.scsid
                        LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=os.scsid
                        LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid
                        LEFT JOIN demandas.sistemadetalhe sd on sd.sidid = ss.sidid

                    where d.esdid = {$esdid}
                    and os.tosid in (" . $tosid . ")
                    $AndEmpresa
                    $andPeriodoOS
                    $filtraSSAnsid
                    order by campoOrdenar desc";

        //ver($sql);
        //dbg($sql,1);
        
        $dadosArr = $db->carregar($sql);
        $dados = array();
        $i = 0;
        $pf = 0;

        foreach ($dadosArr as $dado) {
        	
        	$dados[$i]['odsid'] = $dado['odsid'] <> "" ? $dado['odsid'] : '<center> - </center>';
            $dados[$i]['scsid'] = $dado['scsid'] <> "" ? $dado['scsid'] : '<center> - </center>';
            $dados[$i]['sidabrev'] = $dado['sidabrev'] <> "" ? $dado['sidabrev'] : '<center> - </center>';
            $dados[$i]['dtini'] = $dado['dtini'] <> "" ? $dado['dtini'] : '<center> - </center>';
            $dados[$i]['dtfim'] = $dado['dtfim'] <> "" ? $dado['dtfim'] : '<center> - </center>';
            $dados[$i]['qtd'] = $dado['qtd'] > 0 ? $dado['qtd'] : '<center> - </center>';
            $dados[$i]['valor'] = $dado['valor'] > 0 ? $dado['valor'] : '<center> - </center>';
            $i++;
        }
        
        $cabecalho = array("Nº OS", "Nº SS", "SIGLA", "Prev. Início", "Prev. Término", "Qtd. PF", "Valor (R$)");
        $db->monta_lista_simples($dados, $cabecalho, 100, 50, "N", "100%");
        //$db->monta_lista_array($dados,$cabecalho,100,10,'S','center','','');
    }

    function exibirFinanceiroEmpresas($ctrid) {
        global $db;
        
        if ($ctrid) {
            $Empresa = "AND pfe.id_contrato = $ctrid";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodo = " AND 
                                   ((to_char(pfe.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(pfe.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' )) ";
        }
        
	    if ($_REQUEST['tipoSituacao'] == 1) {//Desenvolvimento
	            $flag_OS_D = "AND id_status_documento not in (257,301,302,377,303,277)";
	        } elseif ($_REQUEST['tipoSituacao'] == 2) {//Utilizada
	            $flag_OS_U = "AND id_status_documento in (257,302,277)";
	        }      
        //Verifica o perãodo do contrato
					$query	 = "SELECT	pfe.id_contrato
								,pfe.id_ss
								,pfe.id_os
								,pfe.id_os_pai
								,pfe.tosid_os
								,pfe.id_status_documento
								,pfe.*
							FROM	fabrica.vw_painel_financeiro_empresas pfe ";
        
        $query_empresa1 = $query." WHERE pfe.tipo_empresa = 'Empresa 1' $andPeriodo $flag_OS_D $flag_OS_U ORDER BY 1, 2 ASC";
        
        $query_empresa2 = $query." WHERE pfe.tipo_empresa = 'Empresa 2' $andPeriodo $flag_OS_D $flag_OS_U ORDER BY 1, 2 ASC";

        $dadosEmpresa1						= $db->carregar( $query_empresa1 );
        $dadosEmpresa2						= $db->carregar( $query_empresa2 );
        
        $dadosEmpresa1 = empty($dadosEmpresa1)==true ? array() :$dadosEmpresa1;
        $dadosEmpresa2 = empty($dadosEmpresa2)==true ? array() :$dadosEmpresa2;
        
        //INICIA OS DADOS DA EMPRESA DO TIPO 1
       foreach( $dadosEmpresa1 as $ss){
       	
       		$id_contrato = $ss['id_contrato'];
       		$id_ss = $ss['id_ss'];
       		$id_status_documento = $ss['id_status_documento'];
       		$glosa = $ss['glosa'];
       		$vl_percentual = $ss['vl_percentual'];
       		
       		if ($id_status_documento == 302){
       			$vl_percentual = 12/100;
       		}

       		//verifica o status do documento se ele for 257 (OS FINALIZADA) e 302 (OS CANCELADA COM CUSTO), realiza a soma do pf utilizado.
       		//Senão realiza a soma do pf em desenvovlvimento
       		if ($id_status_documento == 257 OR $id_status_documento == 302){
       			$somaContrato['pf_utilizado'][$id_contrato] += ($this->verificaMenorValorSS($id_ss, $ss['qtd_pf_estimada'], $ss['qtd_pf_detalhada'], $dadosEmpresa2) 
       															* $vl_percentual) - $glosa;
       			$somaContrato['pf_glosado'][$id_contrato] += $glosa;
       		}elseif ($id_status_documento != 301){
       			//Irá somar todas as OS's que não estejam canceladas sem custo
       			$somaContrato['pf_desenv'][$id_contrato] += ($this->verificaMenorValorSS($id_ss, $ss['qtd_pf_estimada'], $ss['qtd_pf_detalhada'], $dadosEmpresa2)
       														 * $vl_percentual) - $glosa;
       			$somaContrato['pf_glosado'][$id_contrato] += $glosa;
       		}
       }
       //INICIA OS DADOS DA EMPRESA DO TIPO 2
	    foreach( $dadosEmpresa2 as $empresa2){
       		$id_contrato = $empresa2['id_contrato'];
       		$id_ss = $empresa2['id_ss'];
       		$id_status_documento = $empresa2['id_status_documento'];
       		
       		//verifica o status do documento se ele for 277 (OS FINALIZADA), realiza a soma do pf utilizado.
       		//Senão realiza a soma do pf em desenvovlvimento
       		if ($id_status_documento == 277){
       			if ($empresa2['tosid_os'] == 2){
       				$somaContratoEmpresa2['pf_utilizado'][$id_contrato] += $this->retornaMenorValorEstimadaDetalhada($dadosEmpresa1, $id_ss, 'qtd_pf_estimada', $empresa2['qtd_pf_estimada'] );
       			}elseif ($empresa2['tosid_os'] == 3){
       				$somaContratoEmpresa2['pf_utilizado'][$id_contrato] += $this->retornaMenorValorEstimadaDetalhada($dadosEmpresa1, $id_ss, 'qtd_pf_detalhada', $empresa2['qtd_pf_detalhada']);
       			}
       		}elseif ($id_status_documento != 303){
       			//Irá somar todas as OS's que não estejam canceladas sem custo
       			if ($empresa2['tosid_os'] == 2){
       				$somaContratoEmpresa2['pf_desenv'][$id_contrato] += $this->retornaMenorValorEstimadaDetalhada($dadosEmpresa1, $id_ss, 'qtd_pf_estimada', $empresa2['qtd_pf_estimada'] );
       			}elseif ($empresa2['tosid_os'] == 3){
       				$somaContratoEmpresa2['pf_desenv'][$id_contrato] += $this->retornaMenorValorEstimadaDetalhada($dadosEmpresa1, $id_ss, 'qtd_pf_detalhada', $empresa2['qtd_pf_detalhada']);
       			}
       		}
       }

       $arrayFinal = array();

       foreach( $dadosEmpresa1 as $solicitacaoServico){
       	
       		$id_contrato = $solicitacaoServico['id_contrato'];
       		if(!isset($arrayFinal[$id_contrato])){
       				$valor_desenvolvimento = $somaContrato['pf_desenv'][$id_contrato] * $solicitacaoServico["vpcvalor"];
	       			$valor_utilizado = $somaContrato['pf_utilizado'][$id_contrato] * $solicitacaoServico["vpcvalor"];
	       			$pf_desenvolvimento = $somaContrato['pf_desenv'][$id_contrato];
	       			$pf_utilizado = $somaContrato['pf_utilizado'][$id_contrato];
	       			$pf_glosado =  $somaContrato['pf_glosado'][$id_contrato]; 
	       			
       			//FILTRAMOS APENAS AS EMPRESAS DO ITEM 1 PARA AS EMPRESAS DO ITEM VAMOS TIRAR O MAX DO TOSID E REALIZAR UMA NOVA CONSULTA
	       		if ($solicitacaoServico['tipo_empresa']=='Empresa 1'){
		       		$arrayFinal[$id_contrato] = array(
						"numero_contrato" => $solicitacaoServico['ctrnumero'],
		       			"tipo_contrato" => $solicitacaoServico["tipo_empresa"],
		       			"empresa" => $solicitacaoServico["empresa_nome"],
		       			"pf_contratato" => $solicitacaoServico["ctrqtdpfcontrato"],
		       			"valor_por_pf" => $solicitacaoServico["vpcvalor"],
		       			"valor_contratado" => $solicitacaoServico["vl_contratado"],
		       			"pf_desenvolvimento" =>  sprintf("%01.2f",$pf_desenvolvimento),  				
		       			"valor_desenvolvimento" => sprintf("%01.2f",$valor_desenvolvimento),
		       			"pf_utilizado" =>  sprintf("%01.2f",$pf_utilizado),  
		       			"valor_utilizado" =>  sprintf("%01.2f",$valor_utilizado),
		       		    "pf_glosado" =>  sprintf("%01.2f",$pf_glosado),
			       		"pf_disponivel" =>  sprintf("%01.2f",($solicitacaoServico["ctrqtdpfcontrato"] - ($pf_desenvolvimento + $pf_utilizado))),
		       			"valor_disponivel" =>  sprintf("%01.2f",$solicitacaoServico["vl_contratado"] - ($valor_desenvolvimento + $valor_utilizado))
		       		);	
		    	}
       		}
       	
       }
       
	    foreach( $dadosEmpresa2 as $empresa2){
	       	
	       		$id_contrato = $empresa2['id_contrato'];
	       		if(!isset($arrayFinal[$id_contrato])){
	       			$valor_desenvolvimento = $somaContratoEmpresa2['pf_desenv'][$id_contrato] * $empresa2["vpcvalor"];
	       			$valor_utilizado = $somaContratoEmpresa2['pf_utilizado'][$id_contrato] * $empresa2["vpcvalor"];
	       			
	       			//FILTRAMOS APENAS AS EMPRESAS DO ITEM 2 PARA AS EMPRESAS DO ITEM VAMOS TIRAR O MAX DO TOSID E REALIZAR UMA NOVA CONSULTA
							$arrayFinal[$id_contrato] = array(
							"numero_contrato" => $empresa2['ctrnumero'],
			       			"tipo_contrato" => $empresa2["tipo_empresa"],
			       			"empresa" => $empresa2["empresa_nome"],
			       			"pf_contratato" => $empresa2["ctrqtdpfcontrato"],
			       			"valor_por_pf" => $empresa2["vpcvalor"],
			       			"valor_contratado" => $empresa2["vl_contratado"],
			       			"pf_desenvolvimento" =>  sprintf("%01.2f",$somaContratoEmpresa2['pf_desenv'][$id_contrato]),  				
			       			"valor_desenvolvimento" =>  sprintf("%01.2f",$valor_desenvolvimento),
			       			"pf_utilizado" =>  sprintf("%01.2f",$somaContratoEmpresa2['pf_utilizado'][$id_contrato]),  
			       			"valor_utilizado" =>  sprintf("%01.2f",$valor_utilizado),
							 "pf_glosado" =>  sprintf("%01.2f",0),
			       			"pf_disponivel" =>  sprintf("%01.2f",$empresa2["ctrqtdpfcontrato"] - $somaContratoEmpresa2['pf_desenv'][$id_contrato] - $somaContratoEmpresa2['pf_utilizado'][$id_contrato]),
			       			"valor_disponivel" => sprintf("%01.2f",$empresa2["vl_contratado"] - ($valor_desenvolvimento + $valor_utilizado))
			       		);	
	       		}
	       	
	       }
              
       $arrayFinalMesmo = array();
       foreach($arrayFinal as $key => $value){

	       	if ($ctrid != ''){
	       		if ($key == $ctrid){
	       			$arrayFinalMesmo[] = $value;
	       		}
	       	}else{
	       		$arrayFinalMesmo[] = $value;       			
	       	} 
       }
       
        $memorando  = new MemorandoRepositorio();
        $listaMemorando     = $memorando->recupereMemorandoEmitidoGlosadoPorPrestadorServico( PrestadorServico::PRESTADORA_SERVICO_FABRICA );
        $glosaEmpresaItem1  = 0 ;
        
        foreach($listaMemorando as $memo)
        {
            $glosaEmpresaItem1 += $memorando->recuperarValorGlosaMemorando($memo->getId());
        }
        
        $qtdPFMemorando = $glosaEmpresaItem1 / $arrayFinalMesmo[0]['valor_por_pf'];
        $arrayFinalMesmo[0]['pf_glosado'] += $qtdPFMemorando;
        
        $listaMemorando     = $memorando->recupereMemorandoEmitidoGlosadoPorPrestadorServico( PrestadorServico::PRESTADORA_SERVICO_AUDITORA );
        $glosaEmpresaItem2  = 0 ;
        
        foreach($listaMemorando as $memo)
        {
            $glosaEmpresaItem2 += $memorando->recuperarValorGlosaMemorandoEmpresaItem2($memo->getId());
        }
        
        $qtdPFMemorando = $glosaEmpresaItem2 / $arrayFinalMesmo[1]['valor_por_pf'];
        $arrayFinalMesmo[1]['pf_glosado'] += $qtdPFMemorando;
        
                
        $cabecalho = array("Nº Contrato", "Tipo", "Empresa", "P.F. Contratado", "Valor por P.F. (R$)", "Valor Contratado (R$)", "P.F. em Desenvolvimento", "Valor em Desenvolvimento (R$)", "P.F. Utilizado", "Valor Utilizado (R$)","P.F. Glosado", "P.F. Disponível", "Valor Disponível (R$)");
        $db->monta_lista_simples($arrayFinalMesmo, $cabecalho, 100, 50, "N", "100%");
        
    }
    
    private function verificaMenorValorSS($id_ss, $estimada, $detalhada, $empresa2){
    	  	
    	$temp = 0;
		
		//Verifica se existe detalhada
		if ($detalhada!=''){
    		
			$temp = $this->retornaMenorValorEstimadaDetalhada( $empresa2, $id_ss, 'qtd_pf_detalhada', $detalhada );
    		
		//Verifica se existe estimada
    	}else{
    		
    		$temp = $this->retornaMenorValorEstimadaDetalhada( $empresa2, $id_ss, 'qtd_pf_estimada', $estimada );
    	}

		return $temp;
	}
	
	private function retornaMenorValorEstimadaDetalhada( $empresa, $id_ss, $chave, $valor )
	{
		$temp = $valor;

		foreach ($empresa as $ss) {
			if ($ss['id_ss'] == $id_ss) {
    			if ($ss[$chave]!=''){
    				if ($ss[$chave] < $valor) {
    					$temp = $ss[$chave];
    				} 
    			}	    			
    		}
		}
		return $temp;
	}

    public function listarDesciplinasOrdemServico($tpeid = 1) {
        global $db;
        $ansid = $_REQUEST['ansid'];
        //pega tipo
        //$tpeid = 1;
        if ($tpeid)
            $where = "WHERE tpeid = $tpeid";
        $sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
        $tipo = $db->carregar($sql);

        if ($tipo) {


            for ($t = 0; $t <= count($tipo) - 1; $t++) {

                $tpeid = $tipo[$t]['tpeid'];
                $tpedsc = $tipo[$t]['tpedsc'];

                //pega disciplinas
                $sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$ansid}
					AND sp.tpeid = {$tpeid}
					order by 1";
                $disciplina = $db->carregar($sql);

                $txtTd = '';

                if ($disciplina) {

                    for ($j = 0; $j <= count($disciplina) - 1; $j++) {

                        $dspid = $disciplina[$j]['dspid'];

                        $txtTd .= "<b>" . trim($disciplina[$j]['dspdsc']) . "</b><br>";

                        //pega fases
                        $sql = "SELECT distinct f.fasid, f.fasdsc
							FROM fabrica.servicofaseproduto sp
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$ansid}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
                        $fase = $db->carregar($sql);

                        if ($fase) {

                            for ($i = 0; $i <= count($fase) - 1; $i++) {

                                $fasid = $fase[$i]['fasid'];

                                $sql = "SELECT p.prddsc
									FROM fabrica.servicofaseproduto sp
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$ansid}
									and sp.tpeid = {$tpeid}
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
                                $produto = $db->carregarColuna($sql);

                                if ($produto) {
                                    $txtTd .= "<div style='padding-left:20px'><b> - {$fase[$i]['fasdsc']}</b> <div style='padding-left:40px'> - " . implode(";<br> - ", $produto) . ";</div></div>";
                                }
                            }
                        }
                    }//fecha for disciplina
                } else {
                    $txtTd = "N/A";
                }


                return $txtTd;
            }
        }
    }

    function atestarSolicitacao() {
        global $db;

        $scsatesto = $_REQUEST['scsatesto'];
        $scsid = $_REQUEST['scsid'];

        $sql = "update
				fabrica.solicitacaoservico
			set
				scsatesto = " . $scsatesto . ",
				scsdtatesto = now()
			where
				scsid = " . $scsid;
        $db->executar($sql);
        $db->commit();
    }

    function showOrdemServico() {
        global $db;

        $scsid = $_REQUEST['scsid'];

        $sql = "select
				odsid as codigo,
				odsdetalhamento as descricao
			from
				fabrica.ordemservico
			where
				scsid = $scsid
			order by
				odsdetalhamento";

        $db->monta_combo("odsid", $sql, "S", "Selecione...", "", "", "", "", "S");
    }

    function showSolicitacaoServico() {
        global $db;

        $ctrid = $_REQUEST['ctrid'];

        $sql = "	select
					scs.scsid as codigo,
					scs.scsid || ' - ' || substr(scs.scsnecessidade,0,150) || '...' as descricao
				from
					fabrica.solicitacaoservico scs
				inner join
					fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
				where
					ctrid = $ctrid
				and
					scsstatus = 'A'
				order by
					scs.scsnecessidade";

        $db->monta_combo("scsid", $sql, "S", "Selecione...", "exibirOrdemServico", "", "", "", "S");
    }

    function recuperarSolicitacaoPorOrdem($odsid) {
        global $db;
        $sql = "	select
					scsid
				from
					fabrica.ordemservico
				where
					odsid = $odsid";
        return $db->pegaUm($sql);
    }

    function recuperarSistemaPorSolicitacao($scsid) {
        global $db;
        $sql = "	select
					sol.sidid
				from
					fabrica.solicitacaoservico sol
				inner join
					fabrica.analisesolicitacao ans ON ans.scsid = sol.scsid
				where
					sol.scsid = $scsid";

        return $db->pegaUm($sql);
    }

    function mandaEmailOSFinalizada($texto = null, $arrEmailDestino = null) {
        global $db;

        require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
        require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

        $mensagem = new PHPMailer();

        $mensagem->Host = "localhost";
        $mensagem->persistencia = $db;
        $mensagem->Mailer = "smtp";
        $mensagem->FromName = "Fábrica de Software";
        $mensagem->From = "no-reply@mec.gov.br";
        $mensagem->Subject = "Fábrica de Software - ALERTA - Estado do Documento";
        $mensagem->Body = $texto;
        $mensagem->IsHTML(true);


        if (is_array($arrEmailDestino)) {
            foreach ($arrEmailDestino as $k => $email_destino) {
                $mensagem->AddAddress($email_destino);
                $mensagem->Send();
                $mensagem->ClearAddresses();
            }
        }
    }

    function verificaEmpresaContagemPF() {
    	global $db;
        $ctrid = $_REQUEST['ctrid'];

        if ($ctrid) {
            $sql = "select count(ctrid) from fabrica.contrato where ctrcontagem is true and ctrid != $ctrid and ctrstatus = 'A'";
        } else {
            $sql = "select count(ctrid) from fabrica.contrato where ctrcontagem is true and ctrstatus = 'A'";
        }

        $result = $db->pegaUm($sql);

        if ($result) {
            echo 1;
        }
    }

    function exibirFinanceiroEmpresa1SSOS($ctrid, $tpeid = 1, $tpEstimadaDetalhada = 1, $tipoLista = 1) {
        global $db, $total_QtOSSS;

        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND ans.ctrid = $ctrid";
        }


        if ($_REQUEST['tipoSituacao'] == 1) {//Desenvolvimento
            $flag_SS_D = "WHERE esdc.esdid in (246,247,248,249,250,252,361, 512, 511)";
            $flag_OS_D = "WHERE esdc.esdid not in (257,301,302)";
        } elseif ($_REQUEST['tipoSituacao'] == 2) {//Utilizada
            $flag_SS_U = "WHERE esdc.esdid in (253,251)";
            $flag_OS_U = "WHERE esdc.esdid in (257,302)";
        } elseif ($_REQUEST['tipoSituacao'] == 0) {//todos
            $flag_SS_D = "WHERE esdc.esdid in (246,247,248,249,250,252,253,361, 515)";
            $flag_SS_U = " ";
            $flag_OS_D = " WHERE esdc.esdid not in (301,302) and os.tosid = 1";
            $flag_OS_U = " ";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodo = " WHERE
                                 ((to_char(a.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(a.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))
                                 OR
                                 ((to_char(a.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(a.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))";
            $andPeriodoSS = " AND
                                 ((to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))";
            $andPeriodoOS = " AND
                                 ((to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(os.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))";
        }

        $sql = " 
                SELECT 
                        distinct esd.esdid, esd.esddsc, tudo.qtd, tudo.qtdPfEstimada, tudo.vlPfEstimada, tudo.qtdPfDetalhada, tudo.vlPfDetalhada, tudo.vlFinal,tudo.qtdGlosa,
                        (CASE 
                                WHEN esd.esdid = 246 THEN 1 -- ANALISE
                                WHEN esd.esdid = 250 THEN 2 -- EM REVISAO
                                WHEN esd.esdid = 247 THEN 3 -- EM DETALHAMENTO
                                WHEN esd.esdid = 515 THEN 4 -- EM PAUSA (SS)
                                WHEN esd.esdid = 248 THEN 5 -- EM AVALIACAO
                                WHEN esd.esdid = 249 THEN 6 -- EM APROVAção
                                WHEN esd.esdid = 252 THEN 7 -- EM EXECUção
                                WHEN esd.esdid = 361 THEN 8 -- AGUARDANDO PAGAMENTO
                                WHEN esd.esdid = 253 THEN 9 -- SS FINALIZADA                                
                                WHEN esd.esdid = 254 THEN 10  -- CRIADA
                                WHEN esd.esdid = 255 THEN 11 -- EM EXECUção
                                WHEN esd.esdid = 511 THEN 12 -- EM PAUSA (OS) 
                                WHEN esd.esdid = 261 THEN 13 -- EM REVISão
                                WHEN esd.esdid = 256 THEN 14 -- EM AVALIAção
                                WHEN esd.esdid = 513 THEN 15 -- DIVERGÊNCIA DE CONTAGEM
                                WHEN esd.esdid = 264 THEN 16 -- EM APROVAção
                                WHEN esd.esdid = 263 THEN 17 -- EM ATESTO TécNICO
                                WHEN esd.esdid = 512 THEN 18 -- AGUARDANDO PAGAMENTO
                                WHEN esd.esdid = 257 THEN 19 -- FINALIZADA
                        END) as situacao
                FROM
                        workflow.estadodocumento esd
                
                LEFT JOIN
                        (SELECT t.esdid, t.esddsc, count(*) as Qtd, 
                        SUM(odsqtdpfestimada) as qtdPfEstimada,
                        SUM(valorpfestimada) as vlPfEstimada,
                        SUM(odsqtdpfdetalhada) as qtdPfDetalhada,
                        SUM(valorpfdetalhada) as vlPfDetalhada,
                        SUM(glosaqtdepf) as qtdGlosa,
                        SUM(valorfinal) as vlFinal
                        FROM 
                            (select * from 
                                         (SELECT  esdc.esdid, esdc.esddsc, ss.scsid, g.glosaqtdepf, 
                                                (CASE WHEN (os.odsqtdpfestimada > 50 ) THEN
													CASE WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
														AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfestimada 
														OR 
														(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
														AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL) THEN 
														os.odsqtdpfestimada 
													ELSE 
														(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
														AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
													END
												ELSE
														os.odsqtdpfestimada
												END) AS odsqtdpfestimada,
												
                                                (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfestimada,
                                                
												(CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
												AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada OR
												(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
												AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL) THEN 
													os.odsqtdpfdetalhada 
												ELSE 
													(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
													AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
												END) AS odsqtdpfdetalhada, 
												
						                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfdetalhada,
						                         
                                             (CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((
													-- PEGA O MENOR VALOR DA CONTAGEM DETALHADA DA SS
													(CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada) THEN 
																	os.odsqtdpfdetalhada 
														ELSE 
																(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
														END)
													 -- Pega o valor do contrato e o % das disciplinas cotratadas 
													 * ( (SELECT sum(fadvalor)  
													    FROM
														(SELECT
														    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
														 FROM
														 fabrica.analisesolicitacao ans
														 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
														 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
														 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
														 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
														 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
														 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
														 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
														 WHERE
														    ans.scsid=ss.scsid 
														 ORDER BY
														    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
														artefatos))/100 ) 
														-- Retira os PF glosados testando se glosa > 0
													   - CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END)
														* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
													ELSE ((
													-- Pega o menor valor da quantidade estimada
														CASE WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
															AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfestimada) THEN 
															os.odsqtdpfestimada 
														ELSE 
															(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
															AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
														END
														-- Pega o valor do contrato e o % das disciplinas cotratadas 
														* ( (SELECT sum(fadvalor)  
													    FROM
														(SELECT
														    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
														 FROM
														 fabrica.analisesolicitacao ans
														 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
														 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
														 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
														 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
														 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
														 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
														 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
														 WHERE
														    ans.scsid=ss.scsid 
														 ORDER BY
														    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
														artefatos))/100 ) * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
													END) as valorfinal, 
                                                (CASE WHEN esdc.esdid = 246 THEN 1 -- ANALISE
                                                      WHEN esdc.esdid = 250 THEN 2 -- EM REVISAO
                                                      WHEN esdc.esdid = 247 THEN 3 -- EM DETALHAMENTO
                                                      WHEN esdc.esdid = 249 THEN 4 -- EM APROVAção
                                                      WHEN esdc.esdid = 252 THEN 5 -- EM EXECUção
                                                      WHEN esdc.esdid = 361 THEN 6 -- AGUARDANDO PAGAMENTO
                                                      WHEN esdc.esdid = 253 THEN 7 -- FINALIZADA
                                                 END) as situacao, ss.dataabertura, ans.ansprevtermino, os.odsdtprevinicio, os.odsdtprevtermino, ss.docid,
                                                 (SELECT sum(fadvalor)  
                                                        FROM
                                                           (SELECT
                                                               distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                                           FROM
                                                               fabrica.analisesolicitacao ans
                                                           INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                                           INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                                           INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                                           INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                                           INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                                           INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                                           INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                                           WHERE ans.scsid=ss.scsid 
                                                           ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid)
                                                           artefatos) 
                                                as percentual
                FROM fabrica.solicitacaoservico ss
                LEFT JOIN workflow.documento d  ON ss.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                LEFT JOIN fabrica.ordemservico os ON ss.scsid = os.scsid and os.tosid = 1
                LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
                $flag_SS_D
                $flag_SS_U
                AND ss.scsstatus = 'A'
                $AndEmpresa
                
                $andPeriodoSS
                UNION
                SELECT esdc.esdid, esdc.esddsc,os.scsid, g.glosaqtdepf, os.odsqtdpfestimada, 
                        ( (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))as valorpfestimada,
                        os.odsqtdpfdetalhada, 
                        ((SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))as valorpfdetalhada, 
							(CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((os.odsqtdpfdetalhada  
							    * ( (SELECT sum(fadvalor)  
							    FROM
								(SELECT
								    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
								 FROM
								 fabrica.analisesolicitacao ans
								 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
								 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
								 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
								 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
								 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
								 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
								 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
								 WHERE
								    ans.scsid=ss.scsid 
								 ORDER BY
								    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
								artefatos))/100 ) 
								-- Retira os PF glosados testando se glosa > 0
								- CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END)
								* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
							ELSE ((odsqtdpfestimada * ( (SELECT sum(fadvalor)  
							    FROM
								(SELECT
								    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
								 FROM
								 fabrica.analisesolicitacao ans
								 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
								 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
								 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
								 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
								 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
								 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
								 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
								 WHERE
								    ans.scsid=ss.scsid 
								 ORDER BY
								    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
								artefatos))/100 ) * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
							END) as valorfinal,
                        (CASE WHEN esdc.esdid = 254 THEN 8  -- PENDENTE
                              WHEN esdc.esdid = 255 THEN 9  -- EM EXECUção
                              WHEN esdc.esdid = 261 THEN 10 -- EM REVISão
                              WHEN esdc.esdid = 256 THEN 11 -- EM AVALIAção
                              WHEN esdc.esdid = 264 THEN 12 -- EM APROVAção
                              WHEN esdc.esdid = 263 THEN 13 -- EM ATESTO TécNICO
                              WHEN esdc.esdid = 257 THEN 14 -- FINALIZADA
                        END) as situacao, ss.dataabertura, ans.ansprevtermino, os.odsdtprevinicio, os.odsdtprevtermino, os.docid,
                        (SELECT sum(fadvalor)  
                                FROM
                                   (SELECT
                                       distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                   FROM
                                       fabrica.analisesolicitacao ans
                                   INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                   INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                   INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                   INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                   INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                   INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                   INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                   WHERE ans.scsid=ss.scsid 
                                   ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid)
                                   artefatos) 
                        as percentual
                FROM fabrica.ordemservico os
                INNER JOIN workflow.documento d  ON os.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid and os.tosid = 1
                LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid 
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
                $flag_OS_D
                $flag_OS_U
                $AndEmpresa
                
                $andPeriodoOS
                ) a 
                ) t  
                GROUP BY t.esdid, t.esddsc, t.situacao
                ORDER BY t.situacao
                ) tudo on tudo.esdid = esd.esdid 
                WHERE esd.tpdid in ( 26,27 )
                AND esd.esdid NOT IN (301,302,251,300, 261,362,364,262)
                AND esd.esdstatus = 'A'
                ORDER BY situacao";
        //ver($sql);
        //die;
        $dadosSQL = $db->carregar($sql);
        $dados = array();
        $i = 0;
        $total_QtOSSS = 0;

        foreach ($dadosSQL as $dado) {

            $link = '';
            $linhaAdd = '';
            $total_QtOSSS = $dado['qtd'] + $total_QtOSSS;
            if ($dado['qtd'] > 0) {
                if (!$dado['esdid'])
                    $dado['esdid'] = 'x';
                $link = '<a href="javascript:void(0);" onclick="montaSubListaSSOS(\'' . $dado['esdid'] . '\',\'' . $tpEstimadaDetalhada . '\',\'' . $tipoLista . '\',\'' . $ctrid . '\')">
                                <img id="img_mais_' . $tipoLista . $dado['esdid'] . '" src="../imagens/mais.gif" border=0>
                             </a>
                             <a href="javascript:void(0);" onclick="desmontaSubListaSSOS(\'' . $dado['esdid'] . '\',\'' . $tipoLista . '\',\'' . $ctrid . '\')">
                                 <img id="img_menos_' . $tipoLista . $dado['esdid'] . '" src="../imagens/menos.gif" border=0 style="display:none">
                             </a>';
                $linhaAdd = '</tr>
                                 <tr style="background-color:#F7F7F7">
                                    <td colspan="10" style="padding-left:20px;" id="td_' . $tipoLista . $dado['esdid'] . '" ></td>
                                 </tr>';
            }


            if ($dado['esddsc'] == 'Finalizada') {
                $tr = "</tr><tr style='height:0.1px!important;'><td colspan='9' style='background-color:#bbb;'></td></tr><tr>";
            } else {
                $tr = "";
            }

            $dados[$i] = array(
                "situacao" => $link . $dado['esddsc'],
                "Qtd" => $dado['qtd'] > 0 ? $dado['qtd'] : ' <center> - </center> ',
                "qtdPfEstimada" => $dado['qtdpfestimada'] > 0 ? $dado['qtdpfestimada'] : ' <center> - </center> ',
                "vlPfEstimada" => $dado['qtdpfestimada'] > 0 ?  sprintf("%01.2f",($dado['vlpfestimada'] / $dado['qtd']) * $dado['qtdpfestimada']): ' <center> - </center> ',
                "qtdPfDetalhada" => $dado['qtdpfdetalhada'] > 0 ? $dado['qtdpfdetalhada'] : ' <center> - </center> ',
                "vlPfDetalhada" => $dado['qtdpfdetalhada'] > 0 ?  sprintf("%01.2f",($dado['vlpfdetalhada'] / $dado['qtd']) * $dado['qtdpfdetalhada']): ' <center> - </center> ',
                "qtdGlosa" => $dado['qtdglosa'] > 0 ? $dado['qtdglosa'] : ' <center> - </center> ',
                "vlFinal" => $dado['vlfinal'] > 0 ? $dado['vlfinal'] : ' <center> - </center> ',
                "linhaAdd" => $linhaAdd,
                "divisao" => $tr
            );  

            $i++;
        }
        $cabecalho = array("Situação", "Qd. SS/OS", "Qtd. PF Estimado", "Valor PF Estimado (R$)", "Qtd. PF Detalhado", "Valor PF Detalhado (R$)","Qtd Glosa", "Valor a Pagar (R$)", "", "");
        $this->montaListaArray($dados, $cabecalho, count($dados), 50, "N", "100%", null, null, null, $total_QtOSSS);
        //$db->monta_lista_array($dados, $cabecalho, 100, 10, 'S', 'center', '', '');
    }

    function exibirFinanceiroSubListaSSOS($tpeid, $tpEstimadaDetalhada = null, $esdid = null, $ctrid) {
        global $db;

        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND ans.ctrid = $ctrid";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodoSS = " AND
                                 ((to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))";
            $andPeriodoOS = " AND
                                 ((to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(os.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))";
        }

        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false";

            //254,255,null = pendente, execucao e criada - estimadas
            //256,257,261,262,263,264 = detalhada
            //301,302 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidOS = " and esdid is null";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (254,255)";
                $union = "union select null, 'Criada', 1";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (256,257,261,262,263,264)";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (301,302) and os.tosid = 1";
            }
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true";

            //272,273,274,275,276,277 = estimadas ou detalhadas
            //303 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "3";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 4) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "3";
            }
        }
        $vpcvalor = $db->pegaUm($sql);


        $arrPerfil = pegaPerfilGeral();


//permissao PERFIL_PREPOSTO, PERFIL_ESPECIALISTA_SQUADRA
        $botoesContagem = 1;
        if ((in_array(PERFIL_PREPOSTO, $arrPerfil) || in_array(PERFIL_ESPECIALISTA_SQUADRA, $arrPerfil)) && !in_array(PERFIL_SUPER_USUARIO, $arrPerfil)) {
            $botoesContagem = 0;
        }
        //lista as OS
        if ($tpeid == 1) {
            $sql = " SELECT
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        || '<span title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||ss.scsid
                        || CASE 
                                WHEN ans.ansid IS NOT NULL THEN '&ansid=' || ans.ansid
                                ELSE ''
                            END
                        || '\'\">'
                        ||ss.scsid||'</span>'
                        || '</div>' as scsid,
                      '<span style= \"display:none;\">'||lpad(ss.scsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">' as campoOrdenar,
                    
                        (CASE WHEN (os.odsqtdpfestimada > 50 ) THEN
							CASE WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfestimada) THEN 
								os.odsqtdpfestimada 
								WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) < os.odsqtdpfestimada )
								THEN (SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
							ELSE 
								os.odsqtdpfestimada
							END	
						ELSE
							os.odsqtdpfestimada
						END) AS odsqtdpfestimada,
                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfestimada,
                        (CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada) THEN 
								os.odsqtdpfdetalhada 
								WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) < os.odsqtdpfdetalhada )
								THEN (SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
								AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
							ELSE 
								os.odsqtdpfdetalhada
							END) AS odsqtdpfdetalhada,
                          (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A') as valorpfdetalhada,  
                        (SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) 
                        as percentual,         
					(CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((os.odsqtdpfdetalhada
					    * ( (SELECT sum(fadvalor)  
					    FROM
						(SELECT
						    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
						 FROM
						 fabrica.analisesolicitacao ans
						 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
						 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
						 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
						 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
						 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
						 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
						 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
						 WHERE
						    ans.scsid=ss.scsid 
						 ORDER BY
						    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
						artefatos))/100 ) 
						-- Retira os PF glosados testando se glosa > 0
						- CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END)  
						* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
					ELSE ((odsqtdpfestimada * ( (SELECT sum(fadvalor)  
					    FROM
						(SELECT
						    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
						 FROM
						 fabrica.analisesolicitacao ans
						 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
						 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
						 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
						 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
						 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
						 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
						 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
						 WHERE
						    ans.scsid=ss.scsid 
						 ORDER BY
						    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
						artefatos))/100 ) * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
					END) as valorfinal,
                             '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN sd.sidabrev = sd.sidabrev THEN '<p title=\"' || sd.sidabrev || '\">' || substr(sd.sidabrev,0,6) ||'</p>'
                         END)
                     || '</div>' AS sidabrev, ans.mensuravel, g.glosaqtdepf
                FROM fabrica.solicitacaoservico ss
                INNER JOIN workflow.documento d  ON ss.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                LEFT JOIN fabrica.ordemservico os ON ss.scsid = os.scsid and os.tosid = 1
                LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid 
                LEFT JOIN demandas.sistemadetalhe sd on sd.sidid = ss.sidid 
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
                WHERE d.esdid = {$esdid}
                AND ss.scsstatus = 'A'
                $AndEmpresa
                $andPeriodoSS

                UNION
                
                SELECT 
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN os.tosid = " . TIPO_OS_GERAL . " THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||ss.scsid||' / '||os.odsid||'</p>'
                              ELSE
                                   (CASE WHEN esdc.esdid != " . WF_ESTADO_CPF_CANCELADA . " AND 1=$botoesContagem THEN
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                         ELSE
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                    END )
                         END)
                     || '</div>' AS odsid,
                          '<span style= \"display:none;\">'||lpad(os.odsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">' as campoOrdenar,
                     
                        
                        os.odsqtdpfestimada,
                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfestimada,
                        os.odsqtdpfdetalhada ,
                         (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A') as valorpfdetalhada, 
                        
                        --os.odsqtdpfdetalhada, os.odssubtotalpfdetalhada as valorpfdetalhada, 
                        (SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) 
                        as percentual, 
               
			(CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((os.odsqtdpfdetalhada 
			    * ( (SELECT sum(fadvalor)  
			    FROM
				(SELECT
				    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
				 FROM
				 fabrica.analisesolicitacao ans
				 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
				 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
				 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
				 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
				 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
				 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
				 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
				 WHERE
				    ans.scsid=ss.scsid 
				 ORDER BY
				    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
				artefatos))/100 ) 
				-- Retira os PF glosados testando se glosa > 0
				- CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END)  
				* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
			ELSE ((odsqtdpfestimada * ( (SELECT sum(fadvalor)  
			    FROM
				(SELECT
				    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
				 FROM
				 fabrica.analisesolicitacao ans
				 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
				 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
				 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
				 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
				 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
				 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
				 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
				 WHERE
				    ans.scsid=ss.scsid 
				 ORDER BY
				    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
				artefatos))/100 ) * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
			END) as valorfinal,
                        
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN sd.sidabrev = sd.sidabrev THEN '<p title=\"' || sd.sidabrev || '\"> ' || substr(sd.sidabrev,1,6) ||'</p>'
                         END)
                     || '</div>' AS sidabrev, ans.mensuravel, g.glosaqtdepf
 
                FROM fabrica.ordemservico os
                INNER JOIN workflow.documento d  ON os.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                INNER JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid and os.tosid = 1
                INNER JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
                LEFT JOIN demandas.sistemadetalhe sd on sd.sidid = ss.sidid
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
                where d.esdid = {$esdid}
                $AndEmpresa
               
                $andPeriodoOS 
                order by campoOrdenar desc";
        }

        //ver($sql);
        //die;

        $dadosArr = $db->carregar($sql);
        $dados = array();
        $i = 0;
        $pf = 0;


        foreach ($dadosArr as $dado) {

            $dados[$i]['scsid'] = $dado['scsid'] <> "" ? $dado['scsid'] : '<center> - </center>';
            $dados[$i]['sidabrev'] = $dado['sidabrev'] <> "" ? $dado['sidabrev'] : '<center> - </center>';
            if ($dado['mensuravel'] != null) {
                if ($dado['mensuravel'] == "f") {
                    $dados[$i]['mensuravel'] = 'Não';
                } else {
                    $dados[$i]['mensuravel'] = 'Sim';
                }
            } else {
                $dados[$i]['mensuravel'] = ' <center> - </center>';
            }
            
            $odsqtdpfestimada = $dado['odsqtdpfestimada'];
            $valorpfestimada = $dado['valorpfestimada'];
            $dados[$i]['odsqtdpfestimada'] = $odsqtdpfestimada <> "" ? $odsqtdpfestimada : '<center> - </center>';
            $dados[$i]['valorpfestimada'] = $odsqtdpfestimada <> 0  ? number_format($valorpfestimada * $odsqtdpfestimada,2,',','.') : '<center> - </center>';
            
            $odsqtdpfdetalhada = $dado['odsqtdpfdetalhada'];
            //Esse valor sempre será o valor do ponto de funçção do contrato Ex:R$ 352,49
            $valorpfdetalhada = $dado['valorpfdetalhada'];
            
            $dados[$i]['odsqtdpfdetalhada'] = $odsqtdpfdetalhada  <> "" ? $odsqtdpfdetalhada : '<center> - </center>';
            $dados[$i]['valorpfdetalhada'] = $odsqtdpfdetalhada <> 0 ? number_format($valorpfdetalhada * $odsqtdpfdetalhada,2, ',', '.') : '<center> - </center>';
            $percentual = $dado['percentual'];
            $glosaqtdepf = $dado['glosaqtdepf'];
            $dados[$i]['percentual'] = $percentual <> "" ? $percentual : '<center> - </center>';
            $dados[$i]['glosaqtdepf'] = $glosaqtdepf <> "" ? $glosaqtdepf : '<center> - </center>';
            
            if($odsqtdpfdetalhada > 0){
            	$pf= $odsqtdpfdetalhada;
            }else{
            	$pf= $odsqtdpfestimada;
            } 
            $pf = (($pf * ($percentual/100) - $glosaqtdepf ) *  $valorpfdetalhada);
            $dados[$i]['valorfinal'] = $pf <> 0 ? number_format($pf,2, ',', '.') : '<center> - </center>';
            
            $i++;
        }
        //ver($sql);
        //dbg($sql,1);
        $cabecalho = array("SS/OS", "SIGLA", "Mensurável", "Qtd. PF Estimado", "Valor PF Estimado (R$)", "Qtd. PF Detalhado", "Valor PF Detalhado (R$)", "Percentual (%)","Qtd Glosa", "Valor a Pagar (R$)");
        $db->monta_lista_simples($dados, $cabecalho, count($dados), 50, "N", "100%");
    }

    function exibirFinanceiroEmpresa1SSOSCANCELADA($ctrid, $tpeid = 1, $tpEstimadaDetalhada = 1, $tipoLista = 1) {
        global $db;
        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND ans.ctrid = $ctrid";
        }

        if ($_REQUEST['tipoSituacao'] == 1) {//Desenvolvimento
            $flag_SS_D = "WHERE esdc.esdid in (246,247,248,249,250,252,361)";
            $flag_OS_D = "WHERE esdc.esdid not in (257,301,302)";
        } elseif ($_REQUEST['tipoSituacao'] == 2) {//Utilizada
            $flag_SS_U = "WHERE esdc.esdid in (253)";
            $flag_OS_U = "WHERE esdc.esdid in (257,301,302)";
        } elseif ($_REQUEST['tipoSituacao'] == 0) {//todos
            $flag_SS_D = "WHERE esdc.esdid in (251,257,300,301,302)";
            $flag_SS_U = " ";
            $flag_OS_D = " WHERE esdc.esdid  in (301,302) and os.tosid = 1";
            $flag_OS_U = " ";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodo = " WHERE
                                 ((to_char(a.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(a.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))
                                 OR
                                 ((to_char(a.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(a.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))";
            $andPeriodoSS = " AND
                                 ((to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))";
            $andPeriodoOS = " AND
                                 ((to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(os.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))";
        }
                                
          $sql=" 
                SELECT 
						distinct esd.esdid, esd.esddsc, tudo.qtd, tudo.qtdPfEstimada, tudo.vlPfEstimada, tudo.qtdPfDetalhada, tudo.vlPfDetalhada, tudo.vlFinal,tudo.qtdGlosa,
						(CASE 
							WHEN esd.esdid = 251 THEN 1 -- SS - CANCELADA SEM CUSTO 
							WHEN esd.esdid = 300 THEN 2 -- SS - CANCELADA COM CUSTO
							WHEN esd.esdid = 301 THEN 3 -- OS - CANCELADA SEM CUSTO 
							WHEN esd.esdid = 302 THEN 4 -- OS - CANCELADA COM CUSTO 
						END) as situacao
					FROM
						workflow.estadodocumento esd
					INNER JOIN
						workflow.documento doc 
						on doc.esdid=esd.esdid and (doc.tpdid=27 or doc.tpdid=26) and doc.esdid in (301,302,251,300)
					LEFT JOIN
						(SELECT t.esdid, t.esddsc, count(*) as Qtd, 
						SUM(odsqtdpfestimada) as qtdPfEstimada,
						SUM(valorpfestimada) as vlPfEstimada,
						SUM(odsqtdpfdetalhada) as qtdPfDetalhada,
						SUM(valorpfdetalhada) as vlPfDetalhada,
						SUM(glosaqtdepf) as qtdGlosa,
						SUM(valorfinal) as vlFinal
						FROM 
						    (select * from 
								 (SELECT  esdc.esdid, esdc.esddsc, ss.scsid, g.glosaqtdepf, 
									(CASE WHEN (os.odsqtdpfestimada > 50 ) THEN
																CASE WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																	AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfestimada OR
																	(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																	AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL) THEN 
																	os.odsqtdpfestimada 
																ELSE 
																	(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																	AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
																END
															ELSE
																	os.odsqtdpfestimada
															END) AS odsqtdpfestimada,
															
									(SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfestimada,
									
															(CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
															AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada OR
															(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
															AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL) THEN 
																os.odsqtdpfdetalhada 
															ELSE 
																(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
															END) AS odsqtdpfdetalhada, 
															
												(SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfdetalhada,
												 
								     (CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((
																-- PEGA O MENOR VALOR DA CONTAGEM DETALHADA DA SS
																(CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																			AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada OR
																			(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																			AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL) THEN 
																				os.odsqtdpfdetalhada 
																	ELSE 
																			(SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 3 
																			AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
																	END)
																 -- Retira os PF glosados testando se glosa > 0
																 - CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END)
																 -- Pega o valor do contrato e o % das disciplinas cotratadas 
																 * CASE WHEN esdc.esdid = 251 THEN 
																		0.12
																	ELSE
																	 ((SELECT sum(fadvalor)  
																	    FROM
																		(SELECT
																		    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
																		 FROM
																		 fabrica.analisesolicitacao ans
																		 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
																		 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
																		 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
																		 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
																		 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
																		 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
																		 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
																		 WHERE
																		    ans.scsid=ss.scsid 
																		 ORDER BY
																		    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
																		artefatos)/100)
																	END) * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
																ELSE ((
																-- Pega o menor valor da quantidade estimada
																	CASE WHEN ((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																		AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfestimada OR
																		((SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																		AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) IS NULL ) )THEN 
																		os.odsqtdpfestimada 
																	ELSE 
																		(SELECT os2.odsqtdpfestimada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 
																		AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1)
																	END
																	-- Pega o valor do contrato e o % das disciplinas cotratadas 
																	*
																	CASE WHEN esdc.esdid = 300 THEN 
																			0.12
																		ELSE
																		 ((SELECT sum(fadvalor)  
																		    FROM
																			(SELECT
																			    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
																			 FROM
																			 fabrica.analisesolicitacao ans
																			 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
																			 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
																			 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
																			 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
																			 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
																			 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
																			 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
																			 WHERE
																			    ans.scsid=ss.scsid 
																			 ORDER BY
																			    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
																			artefatos)/100)
																		END) 
																	 * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
																END) as valorfinal, 
									(CASE 
										WHEN esdc.esdid = 251 THEN 1 -- SS - CANCELADA SEM CUSTO 
										WHEN esdc.esdid = 300 THEN 2 -- SS - CANCELADA COM CUSTO
									END) as situacao,
									 ss.dataabertura, ans.ansprevtermino, os.odsdtprevinicio, os.odsdtprevtermino, ss.docid,
									 (SELECT sum(fadvalor)  
										FROM
										   (SELECT
										       distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
										   FROM
										       fabrica.analisesolicitacao ans
										   INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
										   INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
										   INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
										   INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
										   INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
										   INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
										   INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
										   WHERE ans.scsid=ss.scsid 
										   ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid)
										   artefatos) 
									as percentual
					FROM fabrica.solicitacaoservico ss
					LEFT JOIN workflow.documento d  ON ss.docid = d.docid
					INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
					LEFT JOIN fabrica.ordemservico os ON ss.scsid = os.scsid and os.tosid = 1
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
					LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
					LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
				    $flag_SS_D
	                $flag_SS_U
	                AND ss.scsstatus = 'A'
	                $AndEmpresa
	                $andPeriodoSS

	                UNION
					
	                SELECT esdc.esdid, esdc.esddsc,os.scsid, g.glosaqtdepf, os.odsqtdpfestimada, 
						( (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))as valorpfestimada,
						os.odsqtdpfdetalhada, 
						((SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))as valorpfdetalhada, 
										(CASE  WHEN os.odsqtdpfdetalhada > 0 THEN (((os.odsqtdpfdetalhada  
										-- Retira os PF glosados testando se glosa > 0
											- CASE WHEN (g.glosaqtdepf > 0) THEN g.glosaqtdepf ELSE 0 END) * 
												CASE WHEN esdc.esdid = 302 THEN 
													0.12
												ELSE
												 ((SELECT sum(fadvalor)  
												    FROM
													(SELECT
													    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
													 FROM
													 fabrica.analisesolicitacao ans
													 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
													 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
													 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
													 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
													 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
													 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
													 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
													 WHERE
													    ans.scsid=ss.scsid 
													 ORDER BY
													    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
													artefatos)/100)
												END)
											* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
										ELSE ((odsqtdpfestimada * 
											CASE WHEN esdc.esdid = 302 THEN 
												0.12
											ELSE
											 ((SELECT sum(fadvalor)  
											    FROM
												(SELECT
												    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
												 FROM
												 fabrica.analisesolicitacao ans
												 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
												 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
												 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
												 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
												 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
												 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
												 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
												 WHERE
												    ans.scsid=ss.scsid 
												 ORDER BY
												    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
												artefatos)/100)
											END)
											* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
										END) as valorfinal,
												(CASE 
										WHEN esdc.esdid = 301 THEN 3 -- OS - CANCELADA SEM CUSTO 
										WHEN esdc.esdid = 302 THEN 4 -- OS - CANCELADA COM CUSTO 
									END) as situacao,
					ss.dataabertura, ans.ansprevtermino, os.odsdtprevinicio, os.odsdtprevtermino, os.docid,
						(SELECT sum(fadvalor)  
							FROM
							   (SELECT
							       distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
							   FROM
							       fabrica.analisesolicitacao ans
							   INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
							   INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
							   INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
							   INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
							   INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
							   INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
							   INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
							   WHERE ans.scsid=ss.scsid 
							   ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid)
							   artefatos) 
						as percentual
					FROM fabrica.ordemservico os
					INNER JOIN workflow.documento d  ON os.docid = d.docid
					INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
					LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid and os.tosid = 1
					LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid 
					LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
					LEFT JOIN fabrica.glosa g ON g.glosaid = os.glosaid
				 	$flag_OS_D
	                $flag_OS_U
	                $AndEmpresa
	                $andPeriodoOS
					) a 
					) t  
					GROUP BY t.esdid, t.esddsc, t.situacao
					ORDER BY t.situacao
					) tudo on tudo.esdid=doc.esdid 
					ORDER BY situacao ";
        //ver($sql);
        //die;
        $dadosSQL = $db->carregar($sql);
        $dados = array();
        $i = 0;
        $total_QtOSSS = 0;

        foreach ($dadosSQL as $dado) {

            $link = '';
            $linhaAdd = '';
             $total_QtOSSS = $dado['qtd'] + $total_QtOSSS;
            if ($dado['qtd'] > 0) {
                if (!$dado['esdid'])
                    $dado['esdid'] = 'x';
                $link = '<a href="javascript:void(0);" onclick="montaSubListaSSOSCANCELADAS(\'' . $dado['esdid'] . '\',\'' . $tpEstimadaDetalhada . '\',\'' . $tipoLista . '\',\'' . $ctrid . '\')">
                                <img id="img_mais_' . $tipoLista . $dado['esdid'] . '" src="../imagens/mais.gif" border=0>
                             </a>
                             <a href="javascript:void(0);" onclick="desmontaSubListaSSOSCANCELADAS(\'' . $dado['esdid'] . '\',\'' . $tipoLista . '\',\'' . $ctrid . '\')">
                                 <img id="img_menos_' . $tipoLista . $dado['esdid'] . '" src="../imagens/menos.gif" border=0 style="display:none">
                             </a>';
                $linhaAdd = '</tr>
                                 <tr style="background-color:#F7F7F7">
                                    <td colspan="7" style="padding-left:20px;" id="td_' . $tipoLista . $dado['esdid'] . '" ></td>
                                 </tr>';
            }
            if ($dado['esddsc'] == 'Cancelada Com Custo') {
                $tr = "</tr><tr style='height:0.1px!important;'><td colspan='9' style='background-color:#bbb;'></td></tr><tr>";
            } else {
                $tr = "";
            }
            $dados[$i] = array(
                "situacao" => $link . $dado['esddsc'],
                "Qtd" => $dado['qtd'] <> "" ? $dado['qtd'] : '<center> - </center>',
                "qtdPfEstimada" => $dado['qtdpfestimada'] <> "" ? $dado['qtdpfestimada'] : '<center> - </center>',
                "vlPfEstimada" => $dado['qtdpfestimada'] <> "" ? sprintf("%01.2f",($dado['vlpfestimada']/$dado['qtd']) * $dado['qtdpfestimada']) : '<center> - </center>',
                "qtdPfDetalhada" => $dado['qtdpfdetalhada'] <> "" ? $dado['qtdpfdetalhada'] : '<center> - </center>',
                "vlPfDetalhada" => $dado['qtdpfdetalhada'] <> "" ? (($dado['vlpfdetalhada']/$dado['qtd']) * $dado['qtdpfdetalhada']) : '<center> - </center>',
                "vlFinal" => $dado['vlfinal'] <> 0 ? $dado['vlfinal'] : '<center> - </center>',
                "linhaAdd" => $linhaAdd,
                "divisao" => $tr
            );

            $i++;
        }

         $cabecalho = array("Situação", "Qd. SS/OS", "Qtd. PF Estimado", "Valor PF Estimado (R$)", "Qtd. PF Detalhado", "Valor PF Detalhado (R$)", "Valor a Pagar (R$)", "", "");
        //$db->monta_lista_array($dados, $cabecalho, 100, 50, "N", "100%");
        $this->montaListaArray($dados, $cabecalho, count($dados), 50, "N", "100%", null, null, null, $total_QtOSSS);
    }

    function exibirFinanceiroSubListaSSOSCANCELADA($tpeid, $tpEstimadaDetalhada = null, $esdid = null, $ctrid) {
        global $db;

        //Verifica o perãodo do contrato
        if ($ctrid) {
            $AndEmpresa = "AND ans.ctrid = $ctrid";
        }

        if ($_REQUEST['ssdtini'] && $_REQUEST['ssdtfim']) {
            $dtinip = formata_data_sql($_REQUEST['ssdtini']);
            $dtfimp = formata_data_sql($_REQUEST['ssdtfim']);
            $andPeriodo = " AND 
                                   (CASE WHEN (SELECT os2.odsdtprevinicio FROM fabrica.ordemservico os2 WHERE os2.tosid = 1 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) is null OR (SELECT os2.odsdtprevtermino FROM fabrica.ordemservico os2 WHERE os2.tosid = 1 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) is null  THEN 
                                       ((to_char(ss.dataabertura, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' OR to_char(ans.ansprevtermino, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp' ))
                                 ELSE
                                    ((to_char(os.odsdtprevinicio, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp'  OR To_char(os.odsdtprevtermino, 'YYYY-MM-DD') BETWEEN '$dtinip' and '$dtfimp' ))
                                 END) ";
        }

        if ($tpeid == 1) {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = false";

            //254,255,null = pendente, execucao e criada - estimadas
            //256,257,261,262,263,264 = detalhada
            //301,302 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            $andEsdidOS = " and esdid is null";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (254,255)";
                $union = "union select null, 'Criada', 1";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (256,257,261,262,263,264)";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (301,302) and os.tosid = 1";
            }
        } else {
            $sql = "SELECT v.vpcvalor FROM fabrica.valorpfcontrato v
			     INNER JOIN fabrica.contrato c ON c.ctrid = v.ctrid
			     where vpcstatus = 'A' and ctrstatus = 'A' and ctrcontagem = true";

            //272,273,274,275,276,277 = estimadas ou detalhadas
            //303 = canceladas
            $campoQtdOS = "os.odsqtdpfestimada";
            if ($tpEstimadaDetalhada == 1) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 2) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (272,273,274,275,276,277)";
                $tosid = "3";
            } elseif ($tpEstimadaDetalhada == 3) {
                $campoQtdOS = "os.odsqtdpfestimada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "2";
            } elseif ($tpEstimadaDetalhada == 4) {
                $campoQtdOS = "os.odsqtdpfdetalhada";
                $andEsdidOS = " and esdid in (303)";
                $tosid = "3";
            }
        }
        $vpcvalor = $db->pegaUm($sql);


        $arrPerfil = pegaPerfilGeral();


//permissao PERFIL_PREPOSTO, PERFIL_ESPECIALISTA_SQUADRA
        $botoesContagem = 1;
        if ((in_array(PERFIL_PREPOSTO, $arrPerfil) || in_array(PERFIL_ESPECIALISTA_SQUADRA, $arrPerfil)) && !in_array(PERFIL_SUPER_USUARIO, $arrPerfil)) {
            $botoesContagem = 0;
        }


        //lista as OS
        if ($tpeid == 1) {
            $sql = " SELECT
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        || '<span title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||ss.scsid||'&ansid='||ans.ansid|| '\'\">'||ss.scsid||'</span>'
                        || '</div>' as scsid
                        , os.odsqtdpfestimada AS odsqtdpfestimada,
                    '<span style= \"display:none;\">'||lpad(ss.scsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">' as campoOrdenar,
                    CASE WHEN d.esdid = 302 THEN
						12
					ELSE
					     (SELECT sum(fadvalor)  
						FROM
						    (SELECT
							distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
						     FROM
						     fabrica.analisesolicitacao ans
						     INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
						     INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
						     INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
						     INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
						     INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
						     INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
						     INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
						     WHERE
							ans.scsid=ss.scsid 
						     ORDER BY
							ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
						    artefatos)
					END as percentual,         
                    (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfestimada,
                        (CASE WHEN (os.odsqtdpfdetalhada > 50 ) THEN
                            CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada) THEN
                                    os.odsqtdpfdetalhada
                            WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) < os.odsqtdpfdetalhada) THEN
                                    (SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid)
                            ELSE
                                    os.odsqtdpfdetalhada
                            END	
                        ELSE
                            os.odsqtdpfdetalhada
                        END) AS odsqtdpfdetalhada, 
                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfdetalhada,
                 (CASE  WHEN os.odsqtdpfdetalhada > 0 THEN ((os.odsqtdpfdetalhada * 
                 CASE WHEN d.esdid = 302 THEN 
						0.12
					ELSE
					 ((SELECT sum(fadvalor)  
					    FROM
						(SELECT
						    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
						 FROM
						 fabrica.analisesolicitacao ans
						 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
						 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
						 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
						 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
						 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
						 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
						 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
						 WHERE
						    ans.scsid=ss.scsid 
						 ORDER BY
						    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
						artefatos)/100)
					END)
					 * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
		ELSE ((odsqtdpfestimada * 
		CASE WHEN d.esdid = 302 THEN 
			0.12
		ELSE
		 ((SELECT sum(fadvalor)  
		    FROM
			(SELECT
			    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
			 FROM
			 fabrica.analisesolicitacao ans
			 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
			 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
			 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
			 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
			 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
			 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
			 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
			 WHERE
			    ans.scsid=ss.scsid 
			 ORDER BY
			    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
			artefatos)/100)
		END)
		* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
                END) as valorfinal,
                    '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN sd.sidabrev = sd.sidabrev THEN '<p title=\"' || sd.sidabrev || '\">' || substr(sd.sidabrev,0,6) ||'</p>'
                         END)
                     || '</div>' AS sidabrev, ans.mensuravel
                    
                FROM fabrica.solicitacaoservico ss
                INNER JOIN workflow.documento d  ON ss.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                LEFT JOIN fabrica.ordemservico os ON ss.scsid = os.scsid and os.tosid = 1
                LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid 
                LEFT JOIN demandas.sistemadetalhe sd on sd.sidid = ss.sidid 
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                WHERE d.esdid = {$esdid}
                AND ss.scsstatus = 'A'
                $AndEmpresa
                $andPeriodo

                UNION
                SELECT 
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN os.tosid = " . TIPO_OS_GERAL . " THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||os.odsid||'</p>'
                              ELSE
                                   (CASE WHEN esdc.esdid != " . WF_ESTADO_CPF_CANCELADA . " AND 1=$botoesContagem THEN
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                         ELSE
                                            '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                                    END )
                         END)
                     || '</div>' AS odsid, os.odsqtdpfestimada AS odsqtdpfestimada, 
                        '<span style= \"display:none;\">'||lpad(os.odsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">' as campoOrdenar,
                        CASE WHEN d.esdid = 302 THEN
							12
						ELSE
						     (SELECT sum(fadvalor)  
							FROM
							    (SELECT
								distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
							     FROM
							     fabrica.analisesolicitacao ans
							     INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
							     INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
							     INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
							     INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
							     INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
							     INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
							     INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
							     WHERE
								ans.scsid=ss.scsid 
							     ORDER BY
								ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
							    artefatos)
						END as percentual,     
                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A') as valorpfestimada,
                                                (CASE WHEN (os.odsqtdpfdetalhada > 50 ) THEN
                            CASE WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) > os.odsqtdpfdetalhada) THEN
                                    os.odsqtdpfdetalhada
                            WHEN ((SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid ORDER BY os2.odsid desc LIMIT 1) < os.odsqtdpfdetalhada) THEN
                                    (SELECT os2.odsqtdpfdetalhada FROM fabrica.ordemservico os2 WHERE os2.tosid = 2 AND os2.scsid=os.scsid)
                            ELSE
                                    os.odsqtdpfdetalhada
                            END	
                        ELSE
                            os.odsqtdpfdetalhada
                        END) AS odsqtdpfdetalhada, 
                        (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A')as valorpfdetalhada,
                        (CASE  WHEN os.odsqtdpfdetalhada > 0 THEN ((os.odsqtdpfdetalhada * 
                         CASE WHEN d.esdid = 302 THEN 
							0.12
						ELSE
						 ((SELECT sum(fadvalor)  
						    FROM
							(SELECT
							    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
							 FROM
							 fabrica.analisesolicitacao ans
							 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
							 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
							 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
							 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
							 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
							 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
							 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
							 WHERE
							    ans.scsid=ss.scsid 
							 ORDER BY
							    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
							artefatos)/100)
						END)
                        * (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
		ELSE ((odsqtdpfestimada * 
		CASE WHEN d.esdid = 302 THEN 
			0.12
		ELSE
		 ((SELECT sum(fadvalor)  
		    FROM
			(SELECT
			    distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
			 FROM
			 fabrica.analisesolicitacao ans
			 INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
			 INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
			 INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
			 INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
			 INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
			 INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
			 INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
			 WHERE
			    ans.scsid=ss.scsid 
			 ORDER BY
			    ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
			artefatos)/100)
		END) 
		* (SELECT vpc.vpcvalor FROM fabrica.valorpfcontrato vpc where vpc.ctrid = ans.ctrid and vpc.vpcstatus = 'A'))
	END) as valorfinal,
                        
                        '<div style=\"white-space: nowrap; color: #0066CC;\">'
                        ||
                        (CASE WHEN sd.sidabrev = sd.sidabrev THEN '<p title=\"' || sd.sidabrev || '\">' || substr(sd.sidabrev,0,6) ||'</p>'
                         END)
                     || '</div>' AS sidabrev, ans.mensuravel
                        
                FROM fabrica.ordemservico os
                INNER JOIN workflow.documento d  ON os.docid = d.docid
                INNER JOIN workflow.estadodocumento esdc ON d.esdid = esdc.esdid
                INNER JOIN fabrica.solicitacaoservico ss ON ss.scsid = os.scsid and os.tosid = 1
                INNER JOIN fabrica.analisesolicitacao ans ON ans.scsid = ss.scsid
                LEFT JOIN demandas.sistemadetalhe sd on sd.sidid = ss.sidid 
                LEFT JOIN fabrica.contrato c ON c.ctrid = ans.ctrid
                where d.esdid = {$esdid}
                $AndEmpresa
                $andPeriodo 
                order by campoOrdenar desc";
        }
        //ver($sql);
        //dbg($sql,1);
        $dadosArr = $db->carregar($sql);
        $dados = array();
        $i = 0;


        foreach ($dadosArr as $dado) {

            $dados[$i]['scsid'] = $dado['scsid'] <> "" ? $dado['scsid'] : '<center> - </center>';
            $dados[$i]['sidabrev'] = $dado['sidabrev'] <> "" ? $dado['sidabrev'] : '<center> - </center>';
            if ($dado['mensuravel'] != null) {
                if ($dado['mensuravel'] == "f") {
                    $dados[$i]['mensuravel'] = 'Não';
                } else {
                    $dados[$i]['mensuravel'] = 'Sim';
                }
            } else {
                $dados[$i]['mensuravel'] = ' <center> - </center>';
            }
            //ver($dado['valorpfestimada']);
            //ver($dado['valorpfdetalhada']);
            $dados[$i]['odsqtdpfestimada'] = $dado['odsqtdpfestimada'] <> "" ? $dado['odsqtdpfestimada'] : '<center> - </center>';
            $dados[$i]['valorpfestimada'] = $dado['odsqtdpfestimada'] <> "" ? sprintf("%01.2f",($dado['valorpfestimada'] * $dado['odsqtdpfestimada'])) : '<center> - </center>';
            $dados[$i]['odsqtdpfdetalhada'] = $dado['odsqtdpfdetalhada'] <> "" ? $dado['odsqtdpfdetalhada'] : '<center> - </center>';
            $dados[$i]['valorpfdetalhada'] = $dado['odsqtdpfdetalhada'] <> "" ? sprintf("%01.2f",($dado['valorpfdetalhada'] * $dado['odsqtdpfdetalhada'])) : '<center> - </center>';
            $dados[$i]['percentual'] = $dado['percentual'] <> "" ? sprintf("%01.2f",($dado['percentual'])) : '<center> - </center>';
            $dados[$i]['valorfinal'] = $dado['valorfinal'] <> "" ? $dado['valorfinal'] : '<center> - </center>';
            $i++;
        }

        //ver($sql);
        //dbg($sql,1);
        //$cabecalho = array("Nº OS","Nº SS","Prev. Início","Prev. Tármino","Qtd. PF","Valor (R$)");
        $cabecalho = array("SS/OS", "SIGLA", "Mensurável", "Qtd. PF Estimado", "Valor PF Estimado (R$)", "Qtd. PF Detalhado", "Valor PF Detalhado (R$)", "Percentual (%)", "Valor a Pagar (R$)");
        $db->monta_lista_simples($dados, $cabecalho, 100, 50, "N", "100%");
    }

}

?>