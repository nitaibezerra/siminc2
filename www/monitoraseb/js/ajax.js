function mostraComboSubAcao( coonid, funcaoCombo, sbaid, cmtid, permissao, cootec ){
    var div_on = document.getElementById( 'sbaid_on' );
    var div_off = document.getElementById( 'sbaid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        if(coonid){
             return new Ajax.Updater(div_on, 'ajax.php',
                     {
                        method: 'post',
                        parameters: '&servico=mostraComboSubAcao&coordenacaoId=' + coonid + '&funcaoCombo=' + funcaoCombo +'&sbaid=' + sbaid+'&permissao=' + permissao+'&cootec=' + cootec,
                        onComplete: function(res){
                            mostraListaAcoes(sbaid,cmtid,coonid, permissao);
                        }
                     });
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            if('mostraListaAcao'==funcaoCombo){
                mostraListaAcoes(null,null,null,permissao);
            }
        }
    }
}

function listar_coordenacao( cootec, coonid, sbaid, cmtid, permissao ){
    var div_on = document.getElementById( 'coonid_on' );
    var div_off = document.getElementById( 'coonid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        if(cootec){
             return new Ajax.Updater(div_on, 'ajax.php',
                     {
                        method: 'post',
                        parameters: '&servico=mostraComboCoordenacao&cootec=' + cootec + '&coonid='+coonid+ '&permissao='+permissao,
                        onComplete: function(res){
                            mostraComboSubAcao(div_on.children.item(0).value, 'mostraListaAcao', sbaid, cmtid, permissao, cootec);
                        }
                     });
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            mostraComboSubAcao(null, 'mostraListaAcao', sbaid, cmtid, permissao);
        }
    }
}

function listar_coordenacao_sem_subacao(cootec, coonid){
    var div_on = document.getElementById( 'coonid_on' );
    if(div_on){
        if(cootec){
             return new Ajax.Updater(div_on, 'ajax.php',
                     {
                        method: 'post',
                        parameters: '&servico=mostraComboCoordenacaoSemSubacao&cootec=' + cootec + '&coonid=' + coonid,
                        onComplete: function(res){ }
                    });
        }else{
            return new Ajax.Updater(div_on, 'ajax.php',
                    {
                       method: 'post',
                       parameters: '&servico=mostraComboCoordenacaoSemSubacao',
                       onComplete: function(res){ }
                   });
        }
    }
}

function mostraComboCursoMestre(coonid, sbaid, cmtid, permissao){
    var div_on = document.getElementById( 'cmtid_on' );
    var div_off = document.getElementById( 'cmtid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        if(coonid && sbaid){
             return new Ajax.Updater(div_on, 'ajax.php',
                     {
                        method: 'post',
                        parameters: '&servico=mostraComboCursoMestre&coonid=' + coonid + '&sbaid=' + sbaid + '&cmtid=' + cmtid + '&permissao='+permissao,
                        onComplete: function(res){ }
                    });
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
        }
    }
}

function mostraListaAcoes(sbaid, cmtid, coonid, permissao){
    cmtid = (cmtid)?cmtid:null;
    var div_on = document.getElementById( 'acaid_on' );
    var div_off = document.getElementById( 'acaid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        if(sbaid){
             return new Ajax.Updater(div_on, 'ajax.php',
                     {     
                        method: 'post',
                        parameters: '&servico=mostraListaAcao&subacaoId=' + sbaid,
                        onComplete: function(res)
                        {
                             mostraComboPublicoAlvoCM(cmtid,permissao,sbaid);
                             mostraComboCursoMestre(coonid, sbaid, cmtid,permissao);
                        }
                    });
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            mostraComboPublicoAlvoCM(cmtid,permissao,sbaid);
            mostraComboCursoMestre(coonid, sbaid, cmtid,permissao);
        }
    }
}

function mostraListaGradeCurricular(itemLista,permissao){
    var div_on = document.getElementById( 'gcrid_on' );
    var div_off = document.getElementById( 'gcrid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        return new Ajax.Updater(div_on, 'ajax.php',
                {
                    method: 'post',
                    parameters: '&servico=mostraListaGradeCurricular&itemLista='+itemLista+'&permissao='+permissao,
                    onComplete: function(res){ }
                });
    }
}

function mostraListaEquipe(itemLista,permissao){
	var div_on = document.getElementById( 'ecmid_on' );
	var div_off = document.getElementById( 'ecmid_off' );        
	div_on.style.display = 'block';
	div_off.style.display = 'none';
	return new Ajax.Updater(div_on, 'ajax.php', {     
		method: 'post',
		parameters: '&servico=mostraListaEquipe&itemLista='+itemLista+'&permissao='+permissao,
		onComplete: function(res)
		{	
		//alert(res.responseText);			 		
		}
	});
}

function mostraListaOfertaVaga(itemLista,permissao){
	var div_on = document.getElementById( 'ovgid_on' );
	var div_off = document.getElementById( 'ovgid_off' );        
	div_on.style.display = 'block';
	div_off.style.display = 'none';
	return new Ajax.Updater(div_on, 'ajax.php', {     
		method: 'post',
		parameters: '&servico=mostraListaOfertaVaga&itemLista='+itemLista+'&permissao='+permissao,
		onComplete: function(res)
		{	
		//alert(res.responseText);			 		
		}
	});
}

function mostraComboPublicoAlvoCM(cmtid, permissao,sbaid){
    var div_on = document.getElementById( 'funid_on' );
    var div_off = document.getElementById( 'funid_off' );
    if(div_on && div_off){
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        if(sbaid){
            return new Ajax.Updater(div_on, 'ajax.php',
                    {
                        method: 'post',
                        parameters: '&servico=mostraComboPublicoAlvoCM&cmtid='+cmtid+'&permissao='+permissao+'&sbaid='+sbaid,
                        onComplete: function(res){ }
                    });
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
        }
    }
}

function atualizaDocumentoAnexo(arqid,donoClass,donoId,permissao){
	var div_on = document.getElementById( 'arqid_on' );
	var div_off = document.getElementById( 'arqid_off' );
	if(div_on && div_off){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		
		return new Ajax.Updater(div_on, 'ajax.php',{
			method: 'post',
			parameters: '&servico=atualizaDocumentoAnexo&arqid='+arqid+'&donoClass='+donoClass+'&donoId='+donoId+'&permissao='+permissao,
			onComplete: function(res){ 
				if(arqid && arqid!=""){
					alert("Documento excluído com sucesso!");
				}
			}
		});
	}
}