<?php
//var_dump(file_exists('../library/jquery/jquery-1.10.2.js'));
//exit;
header("Content-Type: text/html; charset=ISO-8859-1",true);

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once '_funcoes.php';
include_once '_constantes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_POST){
	if($_POST['requisicao'] == 'ok' && $_SESSION['usucpf']){
		header('content-type: text/html; charset=ISO-8859-1');
		$dmdid = salvaDemandaModal();
		$_SESSION['dmdid'] = $dmdid;
		$docid = criarDocumento( $dmdid );
		
		$dados = array();
		$comentario = '';
			
		// Realiza a alteração do estado para em atendimento.
		if($docid){
			wf_alterarEstado( $docid, 184, $comentario, $dados );
		}
		
		unset($_SESSION['dmdid']);
		echo $dmdid;
	} else {
		echo 'erro';
	}
	exit;
}



function salvaDemandaModal(){
	global $db;

	$dmdqtde = '1';
	
	$priid = '3';
	
	$ordid = '23';
	$celid = '49';
	$tipid = '1172'; 
	
	$dmdbacklog = 'f';
	$dmdentrega = 'f';
	
	$dmdatendremoto = 'f';
	$dmdatendurgente = 'f';
	$dmdjudicial = 'f';
	
	$dmdhorarioatendimento = 'C';
	$dmdclassificacao = 'I';
	
	$unaid = '2';
	
		
	$sql = "INSERT INTO demandas.demanda
					(
						tipid,
						celid,
						usucpfdemandante,
						usucpfinclusao,
						usucpfanalise,
						usucpfexecutor,
						dmdtitulo,
						dmddsc,
						dmddatainiprevatendimento,
						dmddatafimprevatendimento,
						priid,
						unaid,
						dmdqtde,
						dmdhorarioatendimento,
						dmdatendremoto,
						dmdatendurgente,
						dmdjudicial,
						dmdbacklog,
						dmdentrega,
						dmdclassificacao,
						dmddatainclusao,
						dmdstatus
					)VALUES(
						".$tipid.",
						".$celid.",
						'".$_SESSION['usucpf']."',
						'".$_SESSION['usucpf']."',
						'".$_SESSION['usucpf']."',
						'".$_POST['usucpfexecutor']."',
						'".utf8_decode($_POST['titulo'])."',
						'".utf8_decode($_POST['descricao'])."',
						'".formata_data_sql($_POST['dmddatainiprevatendimento'])." ".date('H:i:s')."',
						'".formata_data_sql($_POST['dmddatafimprevatendimento'])." 18:00:00',
						".$priid.",
						".$unaid.",
						".$dmdqtde.",
						'".$dmdhorarioatendimento."',
						'".$dmdatendremoto."',
                        '".$dmdatendurgente."',
						'".$dmdjudicial."',
						'".$dmdbacklog."',
						'".$dmdentrega."',
						'".$dmdclassificacao."',
						'".date('Y-m-d H:i:s')."',
						'A'
					) RETURNING dmdid ";

	//dbg($sql,1);			
					
	$dmdid = $db->pegaUm($sql);
	
	$db->commit();
	
	return $dmdid;
}

?>

    

<form id="formdemanda" name="formdemanda" method="post" class="form-horizontal" action="popupCadDemandasGabinete.php?usucpf=<?=$_SESSION['usucpf']?>" >

	<input type="hidden" name="requisicao" value="ok">
	
	<div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs">Demandante:</label>
	    <div class="col-md-10" align="left">
	    	<input type="text" class="form-control" id="demandante" name="demandante" placeholder="Demandante" VALUE="<?=nomeUser($_SESSION['usucpf']);?>" disabled="disabled">
	    </div>
	</div>
	
	<div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs">Título:</label>
	    <div class="col-md-10">
	      <input type="text" class="form-control" id="titulo" name="titulo" placeholder="Título" required="required">
	    </div>
	</div>

	<div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs">Descrição:</label>
	    <div class="col-md-10">
	      <input type="text" class="form-control" id="descricao" name="descricao" placeholder="Descrição" required="required">
	    </div>
	</div>

	<div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs">Previsão Início:</label>
	    <div class="col-md-10">
	      <input name="dmddatainiprevatendimento" type="text" class="form-control data" id="dmddatainiprevatendimento" placeholder="00/00/0000" maxlength="10" value="" data-format="dd/MM/yyyy hh:mm:ss" required="required">
	    </div>
	</div>
	<div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs">Previsão Término:</label>
	    <div class="col-md-10">
	      <input name="dmddatafimprevatendimento" type="text" class="form-control data" id="dmddatafimprevatendimento" placeholder="00/00/0000" maxlength="10" value="" data-format="dd/MM/yyyy hh:mm:ss" required="required">
	    </div>
	</div>
	
	<div class="form-group">
    	<label for="input" class="col-md-2 control-label hidden-xs" >Responsável:</label>
        <div class="col-md-10">
			<?
			$sql = "SELECT DISTINCT
					u.usucpf AS codigo,
					u.usunome AS descricao
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A'  
					AND ur.celid = 49
					AND ur.pflcod = ".DEMANDA_PERFIL_EQUIPE."
				ORDER BY u.usunome	
				";
			$dados = $db->carregar($sql);
			?>
			<select name="usucpfexecutor" id="usucpfexecutor" class="form-control" required="required">
            	<option value=""> Selecione </option>
                	<?php foreach ($dados as $responsavel): ?>
                		<option value="<?php echo $responsavel['codigo'] ?>"><?php echo $responsavel['descricao'] ?></option>
                    <?php endforeach ?>
            </select>
            <p class="help-block"></p>
        </div>
    </div>
    
    <div class="form-group">
	    <label for="input" class="col-md-2 control-label hidden-xs"></label>
	    <div class="col-md-10" align="left">
	      <button id="button-savepo" type="button" class="btn btn-primary">Cadastrar</button>
	    </div>
	</div>
    

</form>




<script language="javascript">


 		$('#dmddatainiprevatendimento').mask('99/99/9999');
        $('#dmddatafimprevatendimento').mask('99/99/9999');
        
        
		$('#button-savepo').click(function() {
		
			var isValid = true;
			$('[required]').each(function(){
				if($(this).val() == ''){
				
					alert('O campo "' + $(this).parent().prev('label').html() + '" é obrigatório');
					$(this).closest('div.form-group').addClass('has-error');
					
					
					$(this).focus();
					isValid = false;
					return false;
				} else {
					$(this).closest('div.form-group').removeClass('has-error');
					}
			});
			
			//valida datas
			var dtini = $('#dmddatainiprevatendimento').val();
			var dtfim = $('#dmddatafimprevatendimento').val();
			
			if(dtini && dtfim){
				var dini = dtini.substring(6, 10) + dtini.substring(3, 5) + dtini.substring(0, 2);		
				var dfim = dtfim.substring(6, 10) + dtfim.substring(3, 5) + dtfim.substring(0, 2);	
				if (parseFloat(dini) > parseFloat(dfim)){
					alert('A data Previsão Término deve ser maior que a data Previsão Início!');
					isValid = false;
					return false;			
				}
			}
			
			if(isValid == true){
				var url = 'popupCadDemandasGabinete.php?usucpf='+$('.btn-primary').attr('usucpf');
				var data = $(this).closest('form').serialize();
				
				
				$.ajax({
				  type: "POST",
				  url: url,
				  data: data,
				  success: function(html){
				  	if(html == 'erro'){
				  	   alert('Erro ao cadastrar. Tente novamente!');
				  	} else {
				  		alert('Demanda Nº '+html+' cadastrada com sucesso!');
				  		window.location.reload();
				  	}
				  	//console.info(html);
				  }
				}, "json");
			}
			
		}); 	
			
</script>	
