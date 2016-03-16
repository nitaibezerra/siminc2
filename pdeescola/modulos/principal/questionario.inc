<?php
	ini_set("memory_limit","1024M"); 
	set_time_limit(0);
	
	include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	include_once APPRAIZ . "pdeescola/classes/controle/CampoExternoControle.class.inc";
	include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
	
	if( $_REQUEST['requisicao'] ){
		$_REQUEST['requisicao']($_REQUEST);
		die();
	}

	if( $_POST['salvar_questionario'] && $_POST['identExterno'] ){
		$obMontaEx = new CampoExternoControle();
		$obMontaEx->salvar();
	}
	
	if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
		$obMonta = new Montatabela();
		$obMonta->salvar();
	}

	#Atualizar Combo Atividade.
	function atualizaComboAtividade( $request ){
		global $db;
		
		extract($request);
	
		if($mtmid != ''){
			$mtmid = " and mtmid = ".$mtmid;
		}else{
			$mtmid = '';
		}
	
		$sql = " select mtaid as codigo, mtadescricao as descricao from pdeescola.metipoatividade where mtaanoreferencia = 2012 $mtmid;";
		echo $db->monta_combo('atividade_'.$posicao, $sql, 'S', 'Selecione Atividade...', '', '', '', 200, 'N', 'atividade_'.$posicao, '', '', '');
	}

	#Gera do Combo do Representante.
	function atualizaComboRepresentante($request){
		global $db;
		
		extract($request);
		
		if($tipo_rep == 'seguimento_1'){
			$id_combo = 'seguimento_1';
		}elseif($tipo_rep == 'seguimento_2'){
			$id_combo = 'seguimento_2';
		}elseif($tipo_rep == 'seguimento_3'){
			$id_combo = 'seguimento_3';
		}
		
		$arr = Array(
				Array('codigo'=>1, 'descricao'=>'Diretor'),
				Array('codigo'=>2, 'descricao'=>'Coodenador Pedagógico'),
				Array('codigo'=>3, 'descricao'=>'Associação de Pais'),
				Array('codigo'=>4, 'descricao'=>'Representante dos Professores'),
				Array('codigo'=>5, 'descricao'=>'Funcionário da Escola'),
				Array('codigo'=>6, 'descricao'=>'Membro do Conselho Escolar'),
				Array('codigo'=>7, 'descricao'=>'Representante Estudantil'),
				Array('codigo'=>8, 'descricao'=>'Representante da Comunidade'),
		);
		echo $db->monta_combo($id_combo, $arr, 'S', 'Selecione...', '','','', '200', 'N', $id_combo, '', '', $title= null);
	}
	
	#Carregar "memid" de acordo com usuário.
	function buscarMemidUsuario(){
		global $db;
		
		$usucpf = $_SESSION['usucpf'];
		
		$sql = "
			Select	ur.usucpf as cpf_cadastrador, 
					ur.entid as entid, 
					e.entnome as nome_escola, 
					e.entcodent as cod_inep, 
					me.memid as memid
			From pdeescola.usuarioresponsabilidade ur
			Inner Join seguranca.perfilusuario pu using(usucpf)
			Inner Join entidade.entidade e using(entid)
			Inner Join pdeescola.memaiseducacao me using(entid)
			Where ur.rpustatus = 'A' and e.entstatus = 'A' and me.memstatus = 'A' and me.memanoreferencia = 2012 and pu.pflcod = 383 and ur.entid is not null and ur.pflcod = pu.pflcod
			and ur.usucpf = '".$usucpf."';
		";
		$dados = $db->pegaLinha($sql);
		return $dados;
	}

	function carregarResposta($qrpid, $perid){
		global $db;
		
		if($qrpid != '' && $perid != ''){
			$sql = "
				Select 	qexid,
						queid,
						perid,
						qrpid,
						resdesc as resdec_cod,
						Case When resdesc = '1' then 'Diretor'
						     When resdesc = '2' then 'Coodenador Pedagógico'
						     When resdesc = '3' then 'Associação de Pais'
						     When resdesc = '4' then 'Representante dos Professores'
						     When resdesc = '5' then 'Funcionário da Escola'
						     When resdesc = '6' then 'Membro do Conselho Escolar'
						     When resdesc = '7' then 'Representante Estudantil'
						     When resdesc = '8' then 'Representante da Comunidade'
						     When cast(tc.mtmid as text) = qe.resdesc Then mtmdescricao
						     When cast(ta.mtaid as text) = qe.resdesc Then mtadescricao
						   else resdesc
						end as resdesc,
						qeord
				From pdeescola.mequestionarioexterno qe
				Left Join pdeescola.metipomacrocampo tc on cast(tc.mtmid as text) = qe.resdesc and mtmanoreferencia = 2012
				Left Join pdeescola.metipoatividade ta on cast(ta.mtaid as text) = qe.resdesc and mtaanoreferencia = 2012
				Where qrpid = $qrpid and perid = $perid;
			";
			$dados = $db->carregar($sql);
		}		
		return $dados;
	}

	function carregarRespostaRecursosPDDE($qrpid, $perid){
		global $db;
	
		if($qrpid != '' && $perid != ''){
			$sql = "
				Select 	q.qexid,
					q.queid,
					q.perid,
					q.qrpid,
					trim(to_char(cast(q.resdesc as numeric), '999G999G999G999D99')) as valor,
					q.qeord,
					rec.total_rec,
					aplic.total_aplic,
					trim(to_char(rec.total_rec - aplic.total_aplic, '999G999G999G999D99')) as total_saldo
				From pdeescola.mequestionarioexterno q
				Join (
					Select 	sum(cast(resdesc as numeric)) as total_rec,
						qrpid,
						perid
					From pdeescola.mequestionarioexterno
					Where qeord = 11 or qeord = 13 or qeord = 15 or qeord = 17
					Group by qrpid, perid
				) as rec on rec.qrpid = q.qrpid and rec.perid = q.perid
				Join (
					Select 	sum(cast(resdesc as numeric)) as total_aplic,
						qrpid,
						perid
					From pdeescola.mequestionarioexterno
					Where qeord = 12 or qeord = 14 or qeord = 18 or qeord = 18
					Group by qrpid, perid
				) as aplic on aplic.qrpid = q.qrpid and aplic.perid = q.perid
				
				Where q.qrpid = $qrpid and q.perid = $perid;
			";
			$dados = $db->carregar($sql);
		}
		return $dados;
	}	
	
	function carregarNomeCoordenador(){
		global $db;
		
		$memid = buscarMemidUsuario();
		
		if($memid != ''){
			$sql = "
				Select	distinct e.entnome as entnome
				From entidade.entidade e
				Join entidade.funcaoentidade fe on e.entid = fe.entid
				Left Join entidade.funentassoc fea on fea.fueid = fe.fueid
				Join pdeescola.memaiseducacao me on fea.entid = me.entid
				Left Join entidade.endereco ed on ed.entid = e.entid
				Join territorios.municipio m on ed.muncod = m.muncod
				Where fe.fuestatus = 'A' and fe.funid = 41 and e.entstatus = 'A' and me.memid = ".$memid['memid']."
				Order by entnome
			";
			$dados = $db->pegaLinha($sql);
			return $dados['entnome'];
		}
	}

	#Busca qrpid relacionado com memid ou entid. 
	function pegaQrpidMaisEd(){
		global $db;
		
		$queid = QUESTIONARIO_MONIT_FISICO_FINANC;
		
		if( !$db->testa_superuser() ){
			
			$entid = buscarMemidUsuario();
			
			if($entid == ''){
				echo "
					<script type=\"text/javascript\">
						alert('O usuário não esta vinculado a uma Entidade, não é possível responder o questionário!');
						window.location = 'pdeescola.php?modulo=inicio&acao=C';
						//history.back(-1);
					</script>
				";
				exit;
			}else{
				$sql = "
					Select	qp.qrpid
					From  pdeescola.pdequestionario qp
					Join questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
					Where qp.memid = {$entid['memid']} and qr.queid = {$queid}
				";
				$qrpid = $db->pegaUm( $sql );
			
				if(!$qrpid){
					$sql = "
						Select ent.entnome
						From entidade.entidade ent
						Where entid = {$entid['memid']}
					";
					$titulo = $db->pegaUm( $sql );
					$arParam = array ( "queid" => $queid, "titulo" => "PDE_ESCOLA-MONITORAMENTO_FISICO_FINANCEIRO (".$titulo.")" );
					$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
					$sql = "INSERT INTO pdeescola.pdequestionario (entid, qrpid, memid) VALUES ({$entid['entid']}, {$qrpid}, {$entid['memid']});";
					$db->executar( $sql );
					$db->commit();
				}
				return $qrpid;
			}
		}else{
			$sql = "Select entid From entidade.entidade Where entnumcpfcnpj = '".$_SESSION['usucpf']."';";
			$entid = $db->pegaUm( $sql );
			//ver($sql, $entid, d);
			if($entid == ''){
				echo "
					<script type=\"text/javascript\">
						alert('O usuário não esta vinculado a uma Entidade, não é possível responder o questionário!');
						window.location = 'pdeescola.php?modulo=inicio&acao=C';
						//history.back(-1);
					</script>
				";
				exit;
			}else{
				$sql = "
					Select	qp.qrpid
					From  pdeescola.pdequestionario qp
					Join questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
					Where qp.entid = {$entid} and qr.queid = {$queid}
				";
				$qrpid = $db->pegaUm( $sql );
					
				if(!$qrpid){
					$arParam = array ( "queid" => $queid, "titulo" => "PDE_ESCOLA-MONITORAMENTO_FISICO_FINANCEIRO (SUPER_USUARIO)" );
					$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
					$sql = "INSERT INTO pdeescola.pdequestionario (entid, qrpid) VALUES ({$entid}, {$qrpid});";
					$db->executar( $sql );
					$db->commit();
				}
				return $qrpid;
			}
		}
	}
	
	function buscaPermissaoAcesso(){
		global $db;

		$habilitado = 'N';
		
		$perfis = arrayPerfil();
		
		if( !$db->testa_superuser() ){
			foreach( $perfis as $perfil ){
				if( $perfil == PDEESC_PERFIL_CAD_MAIS_EDUCACAO){
					$habilitado = 'S';
					break;
				}
			}
		}else{
			$habilitado = 'S';
		}
		return $habilitado;
	}

	function formata_valor_sql($valor){
		$valor = str_replace('.', '', $valor);
		$valor = str_replace(',', '.', $valor);
		return $valor;
	}
	
	include APPRAIZ."includes/cabecalho.inc";
		
	echo'<br>';
	
	monta_titulo( 'Questionário','');
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	jQuery.noConflict();

	function atualizaComboAtividade( mtmid, posicao ){
		var posicao = posicao.replace('macrocampo_','');
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=principal/questionario&acao=A",
			data: "requisicao=atualizaComboatividade&mtmid="+mtmid+"&posicao="+posicao,
			success: function(msg){
				jQuery('#td_atividade_'+posicao).html(msg);
			}
		});
		return true; 
	}
	
	function atualizaComboRepresentante( repres ){
		var repres = repres
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=principal/questionario&acao=A&tipo_rep="+repres,
			data: "requisicao=atualizaComboRepresentante",
			success: function(msg){
				if(repres == 'seguimento_1'){
					jQuery('#representantepedagogico').html(msg);
				}else if(repres == 'seguimento_2'){
					jQuery('#representanteassuntocomunitario').html(msg);
				}else if(repres == 'seguimento_3'){
					jQuery('#representanteexecucaofinanceira').html(msg);
				}
			}
		});
		return true; 
	}
	
	function carregarNomeCoordenador(){
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=principal/questionario&acao=A",
			data: "requisicao=carregarNomeCoordenador&ptrid=",
			async: false,
			success: function(msg){
				alert(msg);
				jQuery('#td_coordernador').html(msg);
			}
		});
	}
</script>

<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
			<fieldset style="width: 94%; background: #fff;"  >
				<legend>Questionário</legend>
				<?php
					$qrpid = pegaQrpidMaisEd(); 	
					$habilitado = buscaPermissaoAcesso();
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilitado) );
				?>
			</fieldset>
		</td>
	</tr>
</table>

